<template>
  <div id="app">
    <h1 class="room-no">T16<span>2018</span></h1>
    <div class="line"></div>
    <h2 class="name">无线门锁系统</h2>
    <token-input
      v-if="showInput"
      v-bind:init-token="initToken"
      v-on:get-token="refreashToken"
    ></token-input>
    <div
      class="check"
      v-if="showImg"
      v-bind:style="{ 'background-image': 'url(' + imgUrl + ')' }"
    ></div>
    <unlock-btn
      v-if="showBtn"
      v-bind:token="initToken"
      v-bind:locked="initLocked"
      v-on:wrong-token="refreashToken"
      v-on:success="showImgFun"
    ></unlock-btn>
  </div>
</template>

<script>
import unlockBtn from "./components/unlockBtn.vue";
import tokenInput from "./components/tokenInput";
let gToken = localStorage.getItem("token");
let gLocked = true;
export default {
  name: "App",
  components: {
    unlockBtn,
    tokenInput,
  },
  data() {
    return {
      initToken: gToken,
      initLocked: gLocked,
      showImg: false,
      showImgFun: () => {
        this.imgUrl = require("./assets/check.gif") + "?" + Math.random();
        this.showImg = true;
        this.showBtn = false;
        setTimeout(() => {
          this.showImg = false;
          this.showBtn = true;
        }, 2000);
      },
      imgUrl: require("./assets/check.gif") + "?" + Math.random(),
      showBtn: localStorage.getItem("token") != null,
      showInput: localStorage.getItem("token") == null,
      refreashToken: () => {
        this.initToken = localStorage.getItem("token");
        if (this.initToken) {
          this.showBtn = true;
          this.showInput = false;
        } else {
          this.showBtn = false;
          this.showInput = true;
        }
      },
    };
  },
};
</script>

<style>
#app {
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #d3e0ec;
  margin-top: 60px;
}
body {
  background-color: #444d58;
}
.room-no {
  margin-top: 5rem;
  font-size: 3rem;
  font-weight: bold;
  margin-bottom: 1rem;
  text-shadow: 5px 3px 20px #282831;
}
.room-no span {
  /* display: block; */
  border-radius: 0.5rem;
  background: #2a3c58;
  padding: 0.2rem;
  margin: 0.2rem;
  color: #59bef2;
}
.line {
  border-top: 1px solid #6e757c;
  width: 12rem;
  margin: auto;
}
.name {
  font-weight: lighter;
}
.unlockBtn {
  position: absolute;
  bottom: 6rem;
  left: 0;
  right: 0;
}
.check {
  background-color: #444d58;
  /* background-image: url(""); */
  /* background-image: url("./assets/check.gif"); */
  /* background-position: bottom -1px right -1px; */
  margin: auto;
  margin-top: 4rem;
  background-blend-mode: lighten;
  background-size: contain;
  background-repeat: no-repeat;
  width: 15rem;
  height: 15rem;
}
</style>
