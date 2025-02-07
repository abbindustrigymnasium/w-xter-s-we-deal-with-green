<template>
  <div class="container">
    <!-- Sensor Data Display -->
    <div class="sensor-display">
      <p class="overviewTxt">
        Temperature: <span>{{ currentData?.temperature || 'N/A' }}°C</span>
      </p>
      <p class="overviewTxt">
        Humidity: <span>{{ currentData?.humidity || 'N/A' }}%</span>
      </p>
    </div>

    <!-- Automation Controls -->
    <div class="upperRow">
      <!-- Fan Automation -->
      <div class="automation-boxes">
        <h3>Fan Auto</h3>
        <label>
          <input type="checkbox" v-model="fanAutomation.enabled" @change="updateFanAutomation" />
          Enabled
        </label>
        <input
          type="number"
          v-model.number="fanAutomation.tempThreshold"
          placeholder="Temp Threshold (°C)"
          @change="updateFanAutomation"
        />
      </div>

      <!-- Hatch Automation -->
      <div class="automation-boxes">
        <h3>Door Auto</h3>
        <label>
          <input
            type="checkbox"
            v-model="hatchAutomation.enabled"
            @change="updateHatchAutomation"
          />
          Enabled
        </label>
        <input
          type="number"
          v-model.number="hatchAutomation.humThreshold"
          placeholder="Humidity Threshold (%)"
          @change="updateHatchAutomation"
        />
      </div>

      <!-- Pump Automation -->
      <div class="automation-boxes">
        <h3>Pump Auto</h3>
        <label>
          <input type="checkbox" v-model="pumpAutomation.enabled" @change="updatePumpAutomation" />
          Enabled
        </label>
        <input
          type="number"
          v-model.number="pumpAutomation.duration"
          placeholder="Duration (min)"
          @change="updatePumpAutomation"
        />
        <input
          type="number"
          v-model.number="pumpAutomation.interval"
          placeholder="Interval (min)"
          @change="updatePumpAutomation"
        />
      </div>
    </div>

    <!-- Device Controls -->
    <div class="bottomRow">
      <!-- Fan Control -->
      <div 
        v-if="fanData" 
        :style="fanAutomation.enabled ? { backgroundColor: '#808080' } : getColor(fanData.status)" 
        class="device-boxes"
      >
        <h3>Fan</h3>
        <p>Status: {{ fanData.status ? 'ON' : 'OFF' }}</p>
        <button 
          @click="toggleFanStatus" 
          :style="btnColor(fanData.status, fanAutomation.enabled)"
          :disabled="fanAutomation.enabled"
        >⏻</button>
        <div v-if="fanData.status">
          <p>Power: {{ fanData.power }}%</p>
          <input
            type="range"
            v-model.number="fanData.power"
            min="0"
            max="100"
            @change="updateFanPower"
            :disabled="fanAutomation.enabled"
          />
        </div>
      </div>

      <!-- Hatch Control -->
      <div 
        v-if="hatchData" 
        :style="hatchAutomation.enabled ? { backgroundColor: '#808080' } : getColor(hatchData.status)" 
        class="device-boxes"
      >
        <h3>Door</h3>
        <p>Status: {{ hatchData.status ? 'OPEN' : 'CLOSED' }}</p>
        <button 
          @click="toggleHatchStatus" 
          :style="btnColor(hatchData.status, hatchAutomation.enabled)"
          :disabled="hatchAutomation.enabled"
        >⏻</button>
      </div>

      <!-- Pump Control -->
      <div 
        v-if="pumpData" 
        :style="pumpAutomation.enabled ? { backgroundColor: '#808080' } : getColor(pumpData.status)" 
        class="device-boxes"
      >
        <h3>Pump</h3>
        <p>Status: {{ pumpData.status ? 'ON' : 'OFF' }}</p>
        <button 
          @click="togglePumpStatus" 
          :style="btnColor(pumpData.status, pumpAutomation.enabled)"
          :disabled="pumpAutomation.enabled"
        >⏻</button>
        <div v-if="pumpData.status">
          <p>Power: {{ pumpData.power }}%</p>
          <input
            type="range"
            v-model.number="pumpData.power"
            min="0"
            max="100"
            @change="updatePumpPower"
            :disabled="pumpAutomation.enabled"
          />
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import { ref, set, get, onValue, db } from '../boot/firebase.js'

export default {
  name: 'controls-page',
  data() {
    return {
      fanData: null,
      hatchData: null,
      pumpData: null,
      currentData: { temperature: 'N/A', humidity: 'N/A' },
      fanAutomation: { enabled: false, tempThreshold: null },
      hatchAutomation: { enabled: false, humThreshold: null },
      pumpAutomation: { enabled: false, duration: null, interval: null },
    }
  },
  methods: {
    // Firebase data fetching
    async fetchDeviceData() {
      try {
        const [fan, hatch, pump] = await Promise.all([
          get(ref(db, '/fan')),
          get(ref(db, '/hatch')),
          get(ref(db, '/pump')),
        ])

        this.fanData = fan.exists() ? fan.val() : null
        this.hatchData = hatch.exists() ? hatch.val() : null
        this.pumpData = pump.exists() ? pump.val() : null
      } catch (error) {
        console.error('Error fetching device data:', error)
      }
    },

    async fetchAutomationSettings() {
      try {
        const [fanAuto, hatchAuto, pumpAuto] = await Promise.all([
          get(ref(db, '/automation/fan')),
          get(ref(db, '/automation/hatch')),
          get(ref(db, '/automation/pump')),
        ])

        if (fanAuto.exists()) {
          this.fanAutomation = {
            enabled: fanAuto.val().enabled,
            tempThreshold: fanAuto.val().tempThreshold,
          }
        }

        if (hatchAuto.exists()) {
          this.hatchAutomation = {
            enabled: hatchAuto.val().enabled,
            humThreshold: hatchAuto.val().humThreshold,
          }
        }

        if (pumpAuto.exists()) {
          this.pumpAutomation = {
            enabled: pumpAuto.val().enabled,
            duration: pumpAuto.val().duration / 60000, // Convert ms to minutes
            interval: pumpAuto.val().interval / 60000,
          }
        }
      } catch (error) {
        console.error('Error fetching automation settings:', error)
      }
    },

    // Automation updates
    async updateFanAutomation() {
      await set(ref(db, '/automation/fan'), {
        enabled: this.fanAutomation.enabled,
        tempThreshold: this.fanAutomation.tempThreshold,
      });
      if (this.fanAutomation.enabled) {
        await set(ref(db, '/fan/status'), false); 
        this.fanData.status = false;
      }
    },

    async updateHatchAutomation() {
      await set(ref(db, '/automation/hatch'), {
        enabled: this.hatchAutomation.enabled,
        humThreshold: this.hatchAutomation.humThreshold,
      });
      if (this.hatchAutomation.enabled) {
        await set(ref(db, '/hatch/status'), false); 
        this.hatchData.status = false;
      }
    },

    async updatePumpAutomation() {
      await set(ref(db, '/automation/pump'), {
        enabled: this.pumpAutomation.enabled,
        duration: this.pumpAutomation.duration * 60000,
        interval: this.pumpAutomation.interval * 60000,
      });
      if (this.pumpAutomation.enabled) {
        await set(ref(db, '/pump/status'), false); 
        this.pumpData.status = false;
      }
    },

    // Existing device control methods
    async toggleFanStatus() {
      const newStatus = !this.fanData.status
      await set(ref(db, '/fan/status'), newStatus)
      this.fanData.status = newStatus
    },

    async updateFanPower() {
      await set(ref(db, '/fan/power'), this.fanData.power)
    },

    async toggleHatchStatus() {
      const newStatus = !this.hatchData.status
      await set(ref(db, '/hatch/status'), newStatus)
      this.hatchData.status = newStatus
    },

    async togglePumpStatus() {
      const newStatus = !this.pumpData.status
      await set(ref(db, '/pump/status'), newStatus)
      this.pumpData.status = newStatus
    },

    async updatePumpPower() {
      await set(ref(db, '/pump/power'), this.pumpData.power)
    },

    // Style methods
    getColor(status) {
      return {
        backgroundColor: status ? '#5EAF5B' : '#E57373',
      }
    },

    btnColor(status, automationEnabled) {
    if (automationEnabled) {
      return { backgroundColor: '#808080' };
    }
    return {
      backgroundColor: status ? '#E57373' : '#5EAF5B',
    };
  },

    // Real-time updates
    setupListeners() {
      onValue(ref(db, '/current'), (snapshot) => {
        this.currentData = snapshot.exists()
          ? snapshot.val()
          : { temperature: 'N/A', humidity: 'N/A' }
      })

      onValue(ref(db, '/automation/fan'), (snapshot) => {
        if (snapshot.exists()) this.fanAutomation = snapshot.val()
      })

      onValue(ref(db, '/automation/hatch'), (snapshot) => {
        if (snapshot.exists()) this.hatchAutomation = snapshot.val()
      })

      onValue(ref(db, '/automation/pump'), (snapshot) => {
        if (snapshot.exists()) {
          this.pumpAutomation = {
            enabled: snapshot.val().enabled,
            duration: snapshot.val().duration / 60000,
            interval: snapshot.val().interval / 60000,
          }
        }
      })
    },
  },
  created() {
    this.fetchDeviceData()
    this.fetchAutomationSettings()
    this.setupListeners()
  },
}
</script>

<style scoped>
.container {
  display: flex;
  width: 100%;
  height: 100%;
  flex-direction: column;
  justify-content: space-evenly;
  align-items: center;
}

.sensor-display {
  display: flex;
  width: 50%;
  height: 10%;
  background-color: rgb(98, 193, 229);
  border-radius: 3rem;
  border: 3px solid rgba(43, 62, 52, 1);
  justify-content: space-evenly;
  align-items: center;
  text-align: center;
}

.upperRow {
  display: flex;
  width: 100%;
  height: 50%;
  flex-direction: row;
  justify-content: space-evenly;
}

.automation-boxes {
  display: flex;
  flex-direction: column;
  justify-content: space-evenly;
  width: 30%;
  background: white;
  border: 3px solid rgba(43, 62, 52, 1);
  border-radius: 3rem;
  transition:
    transform 0.2s ease,
    box-shadow 0.2s ease;
}

.automation-boxes:hover {
  transform: translateY(-2px);
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
}

.automation-boxes input[type='number'] {
  width: 100%;
  border-radius: 3rem;
}

.automation-boxes h3 {
  font-size: 200%;
  text-align: center;
}

.bottomRow {
  display: flex;
  width: 100%;
  height: 50%;
  flex-direction: row;
  justify-content: space-evenly;
}

.device-boxes {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: space-evenly;
  width: 30%;
  border: 3px solid rgba(43, 62, 52, 1);
  border-radius: 3rem;
  transition:
    transform 0.2s ease,
    box-shadow 0.2s ease;
}

.device-boxes:hover {
  transform: translateY(-2px);
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
}

.device-boxes p {
  font-size: 150%;
  text-align: center;
}

button {
  display: flex;
  align-items: center;
  justify-content: center;
  cursor: pointer;
  font-size: 200%;
  width: 45%;
  transition: all 0.2s ease;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
  border: 3px solid rgba(43, 62, 52, 1);
  border-radius: 3rem;
}

button:hover {
  transform: scale(1.1);
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.15);
}

input[type='range'] {
  width: 100%;
}

h3 {
  font-weight: bold;
  font-size: 250%;
}

.overviewTxt span {
  font-weight: 700;
  color: rgba(43, 62, 52, 1);
}

button:disabled {
  cursor: not-allowed;
  opacity: 0.7;
}

input:disabled {
  cursor: not-allowed;
  opacity: 0.7;
}
</style>
