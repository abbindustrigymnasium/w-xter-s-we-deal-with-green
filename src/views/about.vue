<template>
  <div
    id="upperRow"
    style="display: flex; flex-direction: row; justify-content: space-evenly; height: 60%"
  >
    <div class="image-container">
      <img
        src="https://logos-world.net/wp-content/uploads/2023/02/JavaScript-Logo.png"
        alt="Davitspic"
        class="image"
      />
      <div class="overlay">
        <p class="overlay-text">Davit Durgaryan<br />16 years old<br />I like programming</p>
      </div>
    </div>
    <div class="image-container">
      <img
        src="https://img.icons8.com/?size=512&id=13444&format=png"
        alt="Alvinspic"
        class="image"
      />
      <div class="overlay">
        <p class="overlay-text">
          Alvin Gustafsson<br />16 years old<br />I like hardware and Programming
        </p>
      </div>
    </div>
    <div class="image-container">
      <img
        src="https://icons.iconarchive.com/icons/dakirby309/simply-styled/256/Autodesk-Inventor-icon.png"
        alt="Robinspic"
        class="image"
      />
      <div class="overlay">
        <p class="overlay-text">Robin Holmström<br />16 years old<br />I like technology</p>
      </div>
    </div>
  </div>
  <div id="bottomRow" style="display: flex; flex-direction: row; height: 40%">
    <div>
      <p style="text-align: center; font-size: larger; overflow-y: auto">
        Hello! We are group number 8 and consist of three people; <span>Davit Durgaryan</span>,
        <span>Alvin Gustafsson</span> and <span>Robin Holmström</span>. We are the three students
        behind this project!
      </p>
    </div>
    <div>
      <p style="border: 3px solid rgba(43, 62, 52, 1)">
        <b>Davit Durgaryan</b><br /><b>Email:</b> davit.durgaryan@hitachigymnasiet.se<br /><br /><b
          >Alvin Gustafsson</b
        ><br /><b>Email:</b> alvin.gustafsson@hitachigymnasiet.se<br /><br /><b>Robin Holmström</b
        ><br /><b>Email:</b>
        robin.holmstrom@hitachigymnasiet.se
      </p>
    </div>
  </div>
</template>

<script>
export default {
  name: 'about-page',
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
  width: 25%;
  height: 75%;
  overflow: hidden;
}

.image {
  display: block;
  width: 100%;
  height: 100%;
  object-fit: cover;
}

.overlay {
  position: absolute;
  width: 100%;
  height: 100%;
  background-color: rgba(0, 0, 0, 0.6);
  display: flex;
  justify-content: center;
  align-items: center;
  opacity: 0;
  transition: opacity 0.3s ease;
  pointer-events: none;
  border: 3px solid rgb(43, 62, 52);
}

.overlay-text {
  color: white;
  font-size: 200%;
  text-align: center;
  pointer-events: none;
}

.image-container:hover .overlay {
  opacity: 1;
  pointer-events: auto;
}

@media (max-width: 999px) {
  #upperRow {
    display: none !important;
  }
  #bottomRow {
    flex-direction: column !important;
    justify-content: space-evenly;
  }
}

@media (max-height: 529px) {
  #upperRow {
    display: none !important;
  }
  #bottomRow {
    height: 100% !important;
  }
}
</style>
