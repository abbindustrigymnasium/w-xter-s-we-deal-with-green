<template>
  <div
    style="
      display: flex;
      width: 100%;
      height: 100%;
      flex-direction: column;
      justify-content: space-evenly;
      align-items: center;
    "
  >
    <div
      style="
        display: flex;
        width: 70%;
        height: 25%;
        background-color: rgb(254, 254, 254);
        border-radius: 3rem;
        border: 3px solid rgba(43, 62, 52, 1);
        flex-direction: row;
        justify-content: space-evenly;
        text-align: center;
        font-size: var(--bob-font-size);
      "
    >
      <p class="overviewTxt">
        Temperature: <span>{{ currentData?.temperature || 'N/A' }}°C</span>
      </p>
      <p class="overviewTxt">
        Humidity: <span>{{ currentData?.humidity || 'N/A' }}%</span>
      </p>
    </div>
    <div
      style="
        display: flex;
        width: 40%;
        height: auto;
        background-color: rgb(254, 254, 254);
        border-radius: 2rem;
        border: 3px solid rgba(43, 62, 52, 1);
        flex-direction: column;
        justify-content: space-evenly;
        font-size: 1rem;
        padding: 0.8rem;
        margin-top: 1rem;
      "
    >
      <label for="tempThreshold">Temperature Threshold (°C):</label>
      <input
        type="number"
        id="tempThreshold"
        v-model.number="tempThreshold"
        @focus="animateInput"
        @blur="resetInput"
        max="56.7"
        min="-273"
        style="
          width: 100%;
          padding: 0.4rem;
          border-radius: 0.5rem;
          border: 1px solid rgba(43, 62, 52, 1);
          transition: all 0.3s ease;
        "
      />

      <label for="humThreshold" style="margin-top: 0.5rem">Humidity Threshold (%):</label>
      <input
        type="number"
        id="humThreshold"
        v-model.number="humThreshold"
        @focus="animateInput"
        @blur="resetInput"
        min="0"
        max="100"
        maxlength="3"
        style="
          width: 100%;
          padding: 0.4rem;
          border-radius: 0.5rem;
          border: 1px solid rgba(43, 62, 52, 1);
          transition: all 0.3s ease;
        "
      />

      <button
        @click="applyThresholds"
        @mousedown="animateButton"
        @mouseup="resetButton"
        @mouseleave="resetButton"
        style="
          margin-top: 1rem;
          padding: 0.5rem;
          border-radius: 1rem;
          border: 1px solid rgba(43, 62, 52, 1);
          background-color: rgba(94, 175, 91, 1);
          font-weight: bold;
          transition: all 0.2s ease;
        "
      >
        Apply Thresholds
      </button>
    </div>
    <div
      style="
        display: flex;
        width: 100%;
        height: 100%;
        flex-direction: row;
        justify-content: space-evenly;
      "
    >
      <div v-if="fanData" :style="getColor(fanData.status)">
        <label>Fan</label>
        <p>Status: {{ fanData.status ? 'on' : 'off' }}</p>
        <button
          @click="toggleFanStatus"
          :style="btnColor(fanData.status)"
          style="width: 75%; box-sizing: border-box"
        >
          ⏻
        </button>
        <p v-if="fanData.status">Power: {{ fanData.power }}%</p>
        <input
          v-if="fanData.status"
          type="range"
          v-model.number="fanData.power"
          min="0"
          max="100"
          @change="updateFanPower"
        />
      </div>
      <div v-if="hatchData" :style="getColor(hatchData.status)">
        <label>Door</label>
        <p>Status: {{ hatchData.status ? 'open' : 'closed' }}</p>
        <button @click="toggleHatchStatus" :style="btnColor(hatchData.status)" style="width: 75%">
          ⏻
        </button>
      </div>
      <div v-if="pumpData" :style="getColor(pumpIsScheduled)">
        <label>Pump</label>
        <p>Status: {{ pumpIsScheduled ? 'on' : 'off' }}</p>
        <button
          @click="togglePumpStatus"
          :style="btnColor(pumpIsScheduled)"
          style="width: 75%; box-sizing: border-box"
        >
          ⏻
        </button>
        <p v-if="pumpData.status">Power: {{ pumpData.power }}%</p>
        <input
          v-if="pumpData.status"
          type="range"
          v-model.number="pumpData.power"
          min="0"
          max="100"
          @change="updatePumpPower"
        />
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
      tempThreshold: null,
      humThreshold: null,
      pumpIsScheduled: false,
      pumpTimeoutId: null,
    }
  },
  methods: {
    async fetchFanData() {
      try {
        const snapshot = await get(ref(db, '/fan'))
        if (snapshot.exists()) {
          this.fanData = snapshot.val()
        }
      } catch (error) {
        console.error('Error fetching fan data:', error)
      }
    },
    async fetchHatchData() {
      try {
        const snapshot = await get(ref(db, '/hatch'))
        if (snapshot.exists()) {
          this.hatchData = snapshot.val()
        }
      } catch (error) {
        console.error('Error fetching hatch data:', error)
      }
    },
    async fetchPumpData() {
      try {
        const snapshot = await get(ref(db, '/pump'))
        if (snapshot.exists()) {
          this.pumpData = snapshot.val()
          this.pumpIsScheduled = this.pumpData.scheduled || false

          const nextActivationTime = this.pumpData.nextActivationTime
          const now = Date.now()

          if (this.pumpIsScheduled) {
            if (nextActivationTime && nextActivationTime > now) {
              const delay = nextActivationTime - now
              this.pumpTimeoutId = setTimeout(() => {
                this.activatePumpForDuration(10000)
                this.scheduleNextPumpCycle()
              }, delay)
            } else {
              await this.activatePumpForDuration(10000)
              this.scheduleNextPumpCycle()
            }
          }
        }
      } catch (error) {
        console.error('Error fetching pump data:', error)
      }
    },
    async toggleFanStatus() {
      try {
        const newStatus = this.fanData.status === true ? false : true
        await set(ref(db, '/fan/status'), newStatus)
        this.fanData.status = newStatus
      } catch (error) {
        console.error('Error toggling fan status:', error)
      }
    },
    async updateFanPower() {
      try {
        await set(ref(db, '/fan/power'), this.fanData.power)
        console.log(`Fan power updated to: ${this.fanData.power}`)
      } catch (error) {
        console.error('Error updating fan power:', error)
      }
    },
    async toggleHatchStatus() {
      try {
        const newStatus = this.hatchData.status === true ? false : true
        await set(ref(db, '/hatch/status'), newStatus)
        this.hatchData.status = newStatus
      } catch (error) {
        console.error('Error toggling hatch status:', error)
      }
    },
    async togglePumpStatus() {
      try {
        const newScheduledState = !this.pumpIsScheduled
        this.pumpIsScheduled = newScheduledState
        // Update scheduled state in Firebase
        await set(ref(db, '/pump/scheduled'), newScheduledState)

        if (newScheduledState) {
          // Start the pump cycle
          await this.activatePumpForDuration(10000)
          this.scheduleNextPumpCycle()
        } else {
          // Clear scheduled cycles and turn off pump
          clearTimeout(this.pumpTimeoutId)
          await set(ref(db, '/pump/status'), false)
        }
      } catch (error) {
        console.error('Error toggling pump status:', error)
        this.pumpIsScheduled = !this.pumpIsScheduled
      }
    },

    async activatePumpForDuration(duration) {
      try {
        await set(ref(db, '/pump/status'), true)
        setTimeout(async () => {
          if (this.pumpIsScheduled) {
            await set(ref(db, '/pump/status'), false)
          }
        }, duration)
      } catch (error) {
        console.error('Error activating pump:', error)
      }
    },

    async scheduleNextPumpCycle() {
      if (!this.pumpIsScheduled) return

      const nextActivationTime = Date.now() + 24 * 60 * 60 * 1000 // 24 hours
      await set(ref(db, '/pump/nextActivationTime'), nextActivationTime)

      this.pumpTimeoutId = setTimeout(
        () => {
          this.activatePumpForDuration(10000)
          this.scheduleNextPumpCycle()
        },
        24 * 60 * 60 * 1000,
      )
    },

    async updatePumpPower() {
      try {
        await set(ref(db, '/pump/power'), this.pumpData.power)
        console.log(`Pump power updated to: ${this.pumpData.power}`)
      } catch (error) {
        console.error('Error updating pump power:', error)
      }
    },
    getColor(status) {
      return {
        color: 'rgba(43, 62, 52, 1)',
        border: '3px solid rgba(43, 62, 52, 1)',
        height: 'auto',
        borderRadius: '3rem',
        fontWeight: 'bold',
        fontSize: '1.5rem',
        padding: '1rem',
        width: 'auto',
        minWidth: '22%',
        boxSizing: 'border-box',
        backgroundColor: status ? 'rgba(94, 175, 91, 1)' : 'rgba(229, 115, 115, 1)',
      }
    },
    btnColor(status) {
      return {
        backgroundColor: status ? 'rgba(229, 115, 115, 1)' : 'rgba(94, 175, 91, 1)',
        fontWeight: 'bold',
        color: 'rgba(43, 62, 52, 1)',
        borderRadius: '3rem',
      }
    },
    watchCurrentData() {
      try {
        const currentRef = ref(db, '/current')
        onValue(
          currentRef,
          (snapshot) => {
            if (snapshot.exists()) {
              this.currentData = snapshot.val()
              this.checkThresholds()
            } else {
              this.currentData = { temperature: 'N/A', humidity: 'N/A' }
            }
          },
          (error) => {
            console.error('Error watching current data:', error)
          },
        )
      } catch (error) {
        console.error('Error setting up real-time listener:', error)
      }
    },
    async applyThresholds() {
      this.checkThresholds()
    },
    async checkThresholds() {
      if (this.tempThreshold !== null && this.humThreshold !== null) {
        const currentTemp = this.currentData.temperature
        const currentHum = this.currentData.humidity

        if (currentTemp > this.tempThreshold) {
          const tempDifference = currentTemp - this.tempThreshold
          let fanPower = Math.min(100, Math.max(40, (tempDifference / 10) * 100))
          fanPower = Math.round(fanPower)

          if (!this.fanData.status) {
            await this.toggleFanStatus()
          }

          this.fanData.power = fanPower
          await this.updateFanPower()
        } else if (this.fanData.status) {
          await this.toggleFanStatus()
        }

        if (currentHum > this.humThreshold) {
          if (!this.hatchData.status) {
            await this.toggleHatchStatus()
          }
        } else if (this.hatchData.status) {
          await this.toggleHatchStatus()
        }
      }
    },
    animateInput(event) {
      event.target.style.transform = 'scale(1.05)'
      event.target.style.boxShadow = '0 0 8px rgba(94, 175, 91, 0.6)'
    },
    resetInput(event) {
      event.target.style.transform = 'scale(1)'
      event.target.style.boxShadow = 'none'
    },
    animateButton(event) {
      event.target.style.transform = 'scale(0.95)'
    },
    resetButton(event) {
      event.target.style.transform = 'scale(1)'
    },
  },
  created() {
    this.fetchFanData()
    this.fetchHatchData()
    this.fetchPumpData()
    this.watchCurrentData()
  },
}
</script>

<style>
:root {
  --bob-font-size: 3rem;
}

@media (max-width: 1250px) {
  :root {
    --bob-font-size: 2rem;
  }
}
@media (max-width: 600px) {
  :root {
    --bob-font-size: 1rem;
  }
}
</style>
