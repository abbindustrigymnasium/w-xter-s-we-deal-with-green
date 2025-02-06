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
    <div class="automation-grid">
      <!-- Fan Automation -->
      <div class="automation-card">
        <h3>Fan Automation</h3>
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
      <div class="automation-card">
        <h3>Door Automation</h3>
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
      <div class="automation-card">
        <h3>Pump Automation</h3>
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
    <div class="device-grid">
      <!-- Fan Control -->
      <div v-if="fanData" :style="getColor(fanData.status)" class="device-card">
        <h3>Fan</h3>
        <p>Status: {{ fanData.status ? 'ON' : 'OFF' }}</p>
        <button @click="toggleFanStatus" :style="btnColor(fanData.status)">⏻</button>
        <div v-if="fanData.status">
          <p>Power: {{ fanData.power }}%</p>
          <input
            type="range"
            v-model.number="fanData.power"
            min="0"
            max="100"
            @change="updateFanPower"
          />
        </div>
      </div>

      <!-- Hatch Control -->
      <div v-if="hatchData" :style="getColor(hatchData.status)" class="device-card">
        <h3>Door</h3>
        <p>Status: {{ hatchData.status ? 'OPEN' : 'CLOSED' }}</p>
        <button @click="toggleHatchStatus" :style="btnColor(hatchData.status)">⏻</button>
      </div>

      <!-- Pump Control -->
      <div v-if="pumpData" :style="getColor(pumpData.status)" class="device-card">
        <h3>Pump</h3>
        <p>Status: {{ pumpData.status ? 'ON' : 'OFF' }}</p>
        <button @click="togglePumpStatus" :style="btnColor(pumpData.status)">⏻</button>
        <div v-if="pumpData.status">
          <p>Power: {{ pumpData.power }}%</p>
          <input
            type="range"
            v-model.number="pumpData.power"
            min="0"
            max="100"
            @change="updatePumpPower"
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
      })
    },

    async updateHatchAutomation() {
      await set(ref(db, '/automation/hatch'), {
        enabled: this.hatchAutomation.enabled,
        humThreshold: this.hatchAutomation.humThreshold,
      })
    },

    async updatePumpAutomation() {
      await set(ref(db, '/automation/pump'), {
        enabled: this.pumpAutomation.enabled,
        duration: this.pumpAutomation.duration * 60000, // Convert minutes to ms
        interval: this.pumpAutomation.interval * 60000,
      })
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
        border: '3px solid #2B3E34',
        borderRadius: '3rem',
        padding: '1rem',
        width: '60%',
        textAlign: 'center',
      }
    },

    btnColor(status) {
      return {
        backgroundColor: status ? '#E57373' : '#5EAF5B',
        border: '2px solid #2B3E34',
        borderRadius: '10rem',
        padding: '0.5rem',
        margin: '0.5rem',
      }
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

<style>
.container {
  display: flex;
  width: 100%;
  height: 100%;
  flex-direction: column;
  justify-content: space-evenly;
  align-items: center;
  gap: 1.5rem;
  padding: 1rem;
}

.sensor-display {
  display: flex;
  width: 50%;
  height: 10%;
  background-color: white;
  border-radius: 3rem;
  border: 3px solid #2b3e34;
  justify-content: space-evenly;
  align-items: center;
  text-align: center;
  font-size: var(--bob-font-size);
}

.automation-grid {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  gap: 1.5rem;
  width: 50%;
  margin: 0;
  height: 30%;
}

.automation-card {
  background: white;
  border: 2px solid #2b3e34;
  border-radius: 1.5rem;
  padding: 1.2rem;
  display: flex;
  flex-direction: column;
  gap: 0.8rem;
  transition:
    transform 0.2s ease,
    box-shadow 0.2s ease;
}

.automation-card:hover {
  transform: translateY(-2px);
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
}

.automation-card h3 {
  margin: 0 0 0.5rem 0;
  font-size: 1rem;
}

.automation-card label {
  display: flex;
  align-items: center;
  gap: 0.5rem;
  font-size: 0.9rem;
}

.automation-card input[type='number'] {
  width: 100%;
  padding: 0.4rem;
  font-size: 0.9rem;
}

.device-grid {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  gap: 2rem;
  width: 90%;
  height: 50%;
}

.device-card {
  border-radius: 3rem;
  padding: 1.5rem;
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 1rem;
  transition: transform 0.2s ease;
}

.device-card:hover {
  transform: scale(1.02);
}

button {
  cursor: pointer;
  border: none;
  padding: 0.8rem 1.2rem;
  border-radius: 2rem;
  font-size: 1.2rem;
  min-width: 60px;
  min-height: 30px;
  display: flex;
  align-items: center;
  justify-content: center;
  transition: all 0.2s ease;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
}

button:hover {
  transform: scale(1.1);
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.15);
}

input[type='range'] {
  width: 100%;
  margin-top: 0.5rem;
}

.overviewTxt {
  margin: 0;
  font-weight: 500;
}

.overviewTxt span {
  font-weight: 700;
  color: #2b3e34;
}
</style>
