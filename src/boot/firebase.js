import { defineBoot } from '#q-app/wrappers'
import { initializeApp } from 'firebase/app'
import { getDatabase, set, get, onValue, ref } from 'firebase/database'
import {
  getAuth,
  signInWithEmailAndPassword,
  setPersistence,
  browserSessionPersistence,
} from 'firebase/auth'
import { VueFire } from 'vuefire'

const firebaseApp = initializeApp({
  apiKey: 'AIzaSyAv-utnzbanDe6FrsPF4e5U7X4V3IlGr2o',
  authDomain: 'wexteras-g8.firebaseapp.com',
  databaseURL: 'https://wexteras-g8-default-rtdb.europe-west1.firebasedatabase.app',
  projectId: 'wexteras-g8',
  storageBucket: 'wexteras-g8.firebasestorage.app',
  messagingSenderId: '617044379643',
  appId: '1:617044379643:web:30ac1a58dd05ddbdbeed5e',
  measurementId: 'G-SYFCGQFZ2X',
})

const db = getDatabase(firebaseApp)
const auth = getAuth(firebaseApp)

export default defineBoot(async ({ app }) => {
  app.use(VueFire, {
    firebaseApp,
  })
})

export {
  db,
  ref,
  set,
  get,
  onValue,
  auth,
  signInWithEmailAndPassword,
  setPersistence,
  browserSessionPersistence,
}
