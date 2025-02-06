<template>
  <div
    class="upperRow"
    style="
      display: flex;
      flex-direction: row;
      justify-content: space-evenly;
      align-items: center;
      width: 100%;
      height: 20%;
      text-align: center;
      text-decoration: underline;
      padding-top: 2%;
    "
  >
    <h1 id="title">Wexteras</h1>
  </div>
  <div
    class="middleRow"
    style="
      display: flex;
      flex-direction: row;
      justify-content: space-evenly;
      width: 100%;
      height: 30%;
      overflow-y: auto;
    "
  >
    <div class="Txt">
      <h3>Fan</h3>
      <br />
      <p>
        To keep up with the temperature changes we made a fan that can both blow air in and out of
        the greenhouse. You can also adjust the speed, start the fan manually and have it turn on
        and off automatically.
      </p>
    </div>
    <div class="Txt">
      <h3>Door</h3>
      <br />
      <p>
        How can you have a greenHOUSE, without a door? We took it upon ourselves to make said door
        for our little greenhouse so it can live up to its name. The door can turn on and off both
        manually and automatically too.
      </p>
    </div>
    <div class="Txt">
      <h3>Pump</h3>
      <br />
      <p>
        Last but not least, THE PUMP! Watering the plants can sometimes be such a hassle, thus we
        made an automatic watering system which can also be turned on and off both manually and
        automatically.
      </p>
    </div>
  </div>
  <div
    class="bottomRow"
    style="
      display: flex;
      flex-direction: row;
      justify-content: space-evenly;
      width: 100%;
      height: auto;
    "
  >
    <div><img class="Img" src="../assets/Fan.jpg" alt="Picture of the fan system" /></div>
    <div><img class="Img" src="../assets/Door.jpg" alt="Picture of the door system" /></div>
    <div><img class="Img" src="../assets/Pump.jpg" alt="Picture of the pump system" /></div>
  </div>
</template>

<script>
export default {
  name: 'home-page',
}
</script>

<style scoped>
.Img {
  width: 100%;
  height: auto;
}

h3 {
  text-decoration: underline;
}

p {
  width: 100%;
  text-align: center;
  height: 10%;
}

@media (max-width: 500px) {
  #title {
    font-size: 3rem;
  }
  .Txt h3 {
    font-size: 1.5rem;
  }
}
</style>
