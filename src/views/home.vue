<template>
  <div
    class="upperRow"
    style="
      display: flex;
      flex-direction: row;
      justify-content: space-evenly;
      width: 100%;
      height: 50%;
    "
  >
    <div class="Txt">
      <h3>Fan</h3>
      <br />
      <p>
        In the beginning we were completely lost and didn't know what to make or how to make it.
        However, we knew that the deadline was drawing near and we had to hurry up to make something
        presentable.
      </p>
    </div>
    <div class="Txt">
      <h3>Door</h3>
      <br />
      <p>
        Our second achievement was making the door, out of all the groups we are the only one that
        made the door this way. The funny part was that many groups actually doubted the
        functionality of our door until we assembled it and proved them wrong.
      </p>
    </div>
    <div class="Txt">
      <h3>Pump</h3>
      <br />
      <p>
        Last but not least, THE PUMP! This was the most difficult part of them all! We had to buy a
        hose that fit our water pump perfectly so no water got leaked. The pump took so much time to
        code! On our first attempt it just stopped working when the fan was on, and then on the
        second attempt it turned on simultaneously as the fan. We finally got it working on our
        third attempt.
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
      height: 50%;
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
</style>
