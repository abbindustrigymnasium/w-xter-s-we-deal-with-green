<template>
  <div
    style="
      display: flex;
      width: 100%;
      height: 100%;
      flex-direction: column;
      justify-content: space-evenly;
    "
  >
    <div
      style="
        display: flex;
        width: 70%;
        height: auto;
        background-color: rgb(254, 254, 254);
        background-size: contain;
        background-repeat: no-repeat;
        background-position: top center;
        border-radius: 3rem;
        border: 3px solid rgba(43, 62, 52, 1);
        flex-direction: row;
        justify-content: space-evenly;
        font-size: 2rem;
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
        <p v-if="fanData.status">Power: {{ fanData.power }}</p>
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
        <label>Hatch</label>
        <p>Status: {{ hatchData.status ? 'on' : 'off' }}</p>
        <button @click="toggleHatchStatus" :style="btnColor(hatchData.status)" style="width: 75%">
          ⏻
        </button>
      </div>
    </div>
  </div>
</template>

<script>
import {
  ref,
  set,
  get,
  onValue,
} from 'https://www.gstatic.com/firebasejs/11.1.0/firebase-database.js'
import { db } from '../boot/firebase.js'

export default {
  name: 'index-page',
  data() {
    return {
      fanData: null,
      hatchData: null,
      currentData: { temperature: 'N/A', humidity: 'N/A' },
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
  },
  created() {
    this.fetchFanData()
    this.fetchHatchData()
    this.watchCurrentData()
  },
}
</script>