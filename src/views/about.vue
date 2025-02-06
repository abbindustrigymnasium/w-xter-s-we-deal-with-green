<template>
  <div
    style="
      display: flex;
      width: 100%;
      height: 100%;
      flex-direction: column;
      justify-content: flex-start;
      align-items: flex-start;
    "
  >
    <div style="display: flex; width: auto; height: auto">
      <h1 style="font-size: 2rem; text-decoration: underline; transform: rotate(-20deg)">
        Who are we?
      </h1>
    </div>
    <div style="display: flex; flex-direction: row; justify-content: space-evenly">
      <div class="image-container">
        <img :src="images[0]" alt="Davitspic" class="image" />
        <div class="overlay">
          <p class="overlay-text">Davit Durgaryan<br />16 years old<br />I like programming</p>
        </div>
      </div>
      <div class="image-container">
        <img :src="images[1]" alt="Robinspic" class="image" />
        <div class="overlay">
          <p class="overlay-text">Robin Holmström<br />16 years old<br />I like technology</p>
        </div>
      </div>
      <div class="image-container">
        <img :src="images[2]" alt="Alvinspic" class="image" />
        <div class="overlay">
          <p class="overlay-text">Alvin Gustafsson<br />16 years old<br />I like hardware</p>
        </div>
      </div>
    </div>
    <div style="display: flex; flex-direction: row; height: 40%">
      <div>
        <p style="text-align: center; font-size: larger">
          Hello! We are group number 8 and consist of three people; <span>Davit Durgaryan</span>,
          <span>Robin Holmström</span> and <span>Alvin Gustafsson</span>. This project not only
          thaught us a lot but also made us closer to each other as we barely had any prior contact.
          Yet it didn't stop us from performing and completing this project.
        </p>
      </div>
      <div>
        <p style="border: 3px solid rgba(43, 62, 52, 1)">
          <b>Davit Durgaryan</b><br /><b>Email:</b>
          davit.durgaryan@hitachigymnasiet.se<br /><br /><b>Robin Holmström</b><br /><b>Email:</b>
          robin.holmstrom@hitachigymnasiet.se<br /><br /><b>Alvin Gustafsson</b><br /><b>Email:</b>
          alvin.gustafsson@hitachigymnasiet.se
        </p>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'about-page',
  data() {
    return {
      images: [
        'https://logos-world.net/wp-content/uploads/2023/02/JavaScript-Logo.png',
        'https://icons.iconarchive.com/icons/dakirby309/simply-styled/256/Autodesk-Inventor-icon.png',
        'https://cdn.worldvectorlogo.com/logos/arduino-1.svg',
      ],
    }
  },
  methods: {
    hoverImage(index) {
      this.images[index] = this.hoverImages[index]
    },
    resetImage(index) {
      this.images[index] = this.hoverImages[index].replace('_hover', '')
    },
  },
}
</script>

<style scoped>
span {
  font-weight: bold;
}

.image-container {
  position: relative;
  width: 200px;
  height: 200px;
  overflow: hidden;
}

.image {
  width: 100%;
  height: 100%;
  object-fit: cover;
  display: block;
}

.overlay {
  position: absolute;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  background-color: rgba(0, 0, 0, 0.6);
  display: flex;
  justify-content: center;
  align-items: center;
  opacity: 0;
  transition: opacity 0.3s ease;
  padding: 2%;
  pointer-events: none;
  border: 3px solid rgb(43, 62, 52);
}

.overlay-text {
  color: white;
  font-size: 1.2em;
  text-align: center;
  pointer-events: none;
}

.image-container:hover .overlay {
  opacity: 1;
  pointer-events: auto;
}
</style>
