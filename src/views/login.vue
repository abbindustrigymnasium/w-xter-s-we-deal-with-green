<template>
  <div>
    <h1>Login</h1>
    <div style="display: flex; height: 20%; flex-direction: column; justify-content: space-evenly">
      <input
        v-model="email"
        type="email"
        placeholder="Enter email"
        @focus="animateInput"
        @blur="resetInput"
        style="
          width: 40%;
          padding: 0.4rem;
          border-radius: 0.5rem;
          border: 1px solid rgba(43, 62, 52, 1);
          transition: all 0.3s ease;
        "
      />
      <input
        v-model="password"
        type="password"
        placeholder="Enter password"
        @focus="animateInput"
        @blur="resetInput"
        style="
          width: 40%;
          padding: 0.4rem;
          border-radius: 0.5rem;
          border: 1px solid rgba(43, 62, 52, 1);
          transition: all 0.3s ease;
        "
      />
    </div>
    <button
      @click="login"
      style="
        margin-top: 1rem;
        padding: 0.5rem;
        border-radius: 1rem;
        border: 1px solid rgba(43, 62, 52, 1);
        background-color: rgba(94, 175, 91, 1);
        font-weight: bold;
        transition: all 0.2s ease;
      "
      @mousedown="animateButton"
      @mouseup="resetButton"
      @mouseleave="resetButton"
    >
      Submit
    </button>
    <p v-if="error" style="color: red">
      {{ error }}
    </p>
  </div>
</template>

<script>
import {
  auth,
  signInWithEmailAndPassword,
  setPersistence,
  browserSessionPersistence,
} from '../boot/firebase'

export default {
  name: 'LoginPage',
  data() {
    return {
      email: '',
      password: '',
      error: '',
    }
  },
  methods: {
    async login() {
      try {
        await setPersistence(auth, browserSessionPersistence)

        const userCredential = await signInWithEmailAndPassword(auth, this.email, this.password)
        console.log('User logged in:', userCredential.user)
        this.error = ''

        const redirect = this.$route.query.redirect || '/controls'
        this.$router.push(redirect)
      } catch (err) {
        this.error = 'Invalid credentials'
        console.error('Login error:', err.message)
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
}
</script>
