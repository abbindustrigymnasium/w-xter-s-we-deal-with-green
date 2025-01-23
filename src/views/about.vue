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
      <div class="image-container" @mouseover="hoverImage(0)" @mouseleave="resetImage(0)">
        <img :src="images[0]" alt="Davitspic" class="image" />
        <div class="overlay">
          <p class="overlay-text">
            Greetings my fellow kin. I have been bestowed the name Davit by my parents and I would
            dare say that I love programming.
          </p>
        </div>
      </div>
      <div class="image-container" @mouseover="hoverImage(1)" @mouseleave="resetImage(1)">
        <img :src="images[1]" alt="Robinspic" class="image" />
        <div class="overlay">
          <p class="overlay-text">What marvelous times! My name is Robin and I like technology!</p>
        </div>
      </div>
      <div class="image-container" @mouseover="hoverImage(2)" @mouseleave="resetImage(2)">
        <img :src="images[2]" alt="Alvinspic" class="image" />
        <div class="overlay">
          <p class="overlay-text">Alvin's text here</p>
        </div>
      </div>
    </div>
    <div>
      <p style="text-align: center; width: 50%; font-size: larger">
        Hello! We are group number 8 and consist of three people; <span>Davit Durgaryan</span>,
        <span>Robin Holmström</span> and <span>Alvin Gustafsson</span>. This project not only
        thaught us a lot but also made us closer to each other as we barely had any prior contact.
        Yet it didn't stop us from performing and completing this project.
      </p>
    </div>
  </div>
</template>

<script>
export default {
  name: 'about-page',
  data() {
    return {
      images: [
        '../assets/Davit.jpg', // Default image for Davit
        '../assets/Robin2.jpg', // Default image for Robin
        'https://cdn.pixabay.com/photo/2024/03/28/04/22/ai-generated-8660235_640.png', // Default image for Alvin
      ],
      hoverImages: [
        '../assets/Davit_hover.jpg', // Hover image for Davit
        '../assets/Robin_hover.jpg', // Hover image for Robin
        'https://cdn.pixabay.com/photo/2024/03/28/05/22/ai-generated-8670235_640.png', // Hover image for Alvin
      ],
    }
  },
  methods: {
    hoverImage(index) {
      this.images[index] = this.hoverImages[index]
    },
    resetImage(index) {
      this.images[index] = this.hoverImages[index].replace('_hover', '') // Reset logic
    },
  },
}
</script>

<style>
/* Same styles as before */
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
