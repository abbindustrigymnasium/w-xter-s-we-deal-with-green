<template>
  <div id="app">
    <nav>
      <router-link class="naver" to="/">Home</router-link>
      <router-link class="naver" to="/controls">Overview</router-link>
      <router-link class="naver" to="/about">About Us</router-link>
    </nav>

    <main>
      <router-view />
    </main>
  </div>
</template>

<style>
@import url('https://fonts.googleapis.com/css2?family=Varela+Round&display=swap');

* {
  font-family: 'Varela Round', sans-serif;
  color: rgba(43, 62, 52, 1);
  margin: 0;
}

p {
  margin: 0;
}

html,
body {
  margin: 0;
  padding: 0;
  height: 100%;
  width: 100%;
  background-color: rgb(213 205 188);
}

main {
  display: flex;
  height: 100%;
  width: 100%;
  flex-direction: column;
  align-items: center;
}

div {
  display: flex;
  height: 100%;
  width: 100%;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  padding: 0.5%;
  box-sizing: border-box;
}

nav {
  display: flex;
  background-color: transparent;
  border-radius: 2rem;
  border: 3px solid rgb(43, 62, 52);
  width: 100%;
  height: 5vh;
  justify-content: space-around;
  align-items: center;
  background-color: rgba(94, 175, 91, 1);
}

.naver {
  font-weight: bold;
  font-size: large;
  color: rgba(43, 62, 52, 1);
  text-decoration: none;
  transition: ease 0.3s;
}

.naver:hover {
  transform: scale(1.2);
  transition: ease 0.3s;
  text-decoration: underline;
}

label {
  font-size: larger;
  text-decoration: underline;
}

.router-link-exact-active {
  font-size: larger;
  text-decoration: underline;
  color: black;
}
</style>
