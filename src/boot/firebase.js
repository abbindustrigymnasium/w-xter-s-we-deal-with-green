// Import Firebase modules directly from the CDN
import { initializeApp } from 'https://www.gstatic.com/firebasejs/11.1.0/firebase-app.js'
import {
  getDatabase,
  ref,
  set,
  get,
} from 'https://www.gstatic.com/firebasejs/11.1.0/firebase-database.js'

const firebaseConfig = {
  apiKey: 'AIzaSyAv-utnzbanDe6FrsPF4e5U7X4V3IlGr2o',
  authDomain: 'wexteras-g8.firebaseapp.com',
  databaseURL: 'https://wexteras-g8-default-rtdb.europe-west1.firebasedatabase.app',
  projectId: 'wexteras-g8',
  storageBucket: 'wexteras-g8.firebasestorage.app',
  messagingSenderId: '617044379643',
  appId: '1:617044379643:web:30ac1a58dd05ddbdbeed5e',
  measurementId: 'G-SYFCGQFZ2X',
}

// Initialize the Firebase app
const firebaseApp = initializeApp(firebaseConfig)

// Initialize Firebase Realtime Database
const db = getDatabase(firebaseApp)

// Export the database instance and helper functions
export { db, ref, set, get }
