<template>
  <div class="unlockBtn">
    <!-- <button
      id="unlock-btn"
      v-on:click="unlock"
      v-bind:class="{ locked: locked, unlocked: !locked }"
    >
      解 锁
    </button> -->

    <div class="hint-info" v-if="showMsg">{{ msg }}</div>
    <drag-verify
      v-if="hackReset"
      :width="width"
      :height="height"
      :text="text"
      :success-text="successText"
      :background="background"
      :progress-bar-bg="progressBarBg"
      :completed-bg="completedBg"
      :handler-bg="handlerBg"
      :handler-icon="handlerIcon"
      :text-size="textSize"
      :success-icon="successIcon"
      :circle="getShape"
      v-on:passcallback="unlock"
    ></drag-verify>
  </div>
</template>

<script>
import axios from "axios";
import dragVerify from "vue-drag-verify";
export default {
  props: ["token"],
  components: {
    dragVerify,
  },
  data() {
    return {
      handlerIcon: "fas fa-angle-right fa-7x",
      successIcon: "",
      background: "#203f50",
      progressBarBg: "#4c9bc7",
      completedBg: "#4c9bc7",
      handlerBg: "linear-gradient(145deg, rgb(24 56 74), rgb(12 107 140))",
      text: "",
      successText: "",
      width: 320,
      height: 80,
      textSize: "22px",
      isCircle: "true",
      color: "#7fc6e8",
      hackReset: true,
      msg: "",
      showMsg: false,
      unlock: () => {
        // 震动
        if (window.navigator.vibrate) {
          window.navigator.vibrate(60);
        } else if (window.navigator.webkitVibrate) {
          window.navigator.webkitVibrate(60);
        }
        // ajax
        axios({
          url: "/api/unlock",
          data: { data: this.token },
          method: "post",
        })
          .then(() => {
            // 成功
            this.$emit("success");
            setTimeout(() => {
              // 重置
              this.hackReset = false;
              this.$nextTick(() => {
                this.hackReset = true;
              });
            }, 3000);
          })
          .catch((error) => {
            // 失败
            let response = error.response;
            if (response.status == 403) {
              // 密钥错误
              localStorage.removeItem("token");
              this.$emit("wrong-token");
            } else {
              // 其它错误
              if (response.status == 404) {
                // 网络错误
                response.data = { msg: "网络错误" };
              }
              // 重置
              this.hackReset = false;
              this.$nextTick(() => {
                this.hackReset = true;
              });
            }
            // 显示提示字符
            let msg = response.data.msg;
            this.msg = msg;
            this.showMsg = true;
            setTimeout(() => {
              if (this.msg == msg) {
                this.showMsg = false;
                this.msg = "";
              }
            }, 2000);
          });
      },
    };
  },
  computed: {
    getShape() {
      return this.isCircle === "true";
    },
  },
};
document.ontouchmove = function (e) {
  e.preventDefault();
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
.hint-info {
  margin: 1rem auto;
}
#unlock-btn {
  color: #7fc6e8;
  border-radius: 200px;
  width: 12rem;
  height: 12rem;
  border: none;
  box-shadow: 0 8px 16px 0 rgba(0, 0, 0, 0.2);
  outline: none;
  font-size: 2.5rem;
  background-color: #333b4a;
}
#unlock-btn.locked {
  border: 2px solid #7fc6e8;
}
#unlock-btn:active {
  /* border: 2px solid #98d3ee; */
  background-color: #414f64;
}
#unlock-btn.unlocked {
  background: #e0e0e0;
  /* box-shadow: -20px 20px 60px #bebebe, 20px -20px 60px #ffffff; */
}

h3 {
  margin: 40px 0 0;
}
ul {
  list-style-type: none;
  padding: 0;
}
li {
  display: inline-block;
  margin: 0 10px;
}
a {
  color: #42b983;
}
.drag_verify {
  margin: auto;
  position: relative;
}
.drag_verify >>> .dv_handler.dv_handler_bg {
  background: linear-gradient(145deg, rgb(24 56 74), rgb(12 107 140));
  border: 2px solid #4888a6;
  box-shadow: 0 0 16px 0 rgba(0, 255, 255, 0.2);
}
.drag_verify >>> .dv_progress_bar {
  border: 2px solid #2e4d5d;
  min-width: 39px;
}
.drag_verify >>> .dv_text {
  border: 2px solid #2e4d5d;
  border-radius: 50px;
  box-shadow: inset 18px 18px 36px #182f3c, inset -18px -18px 36px #284f64,
    0 8px 16px 0 rgba(0, 0, 0, 0.2);
  color: #7fc6e8;
  overflow: hidden;

  transform: translateZ(0);
}
.drag_verify >>> .dv_text::after {
  content: "";
  background-color: rgba(178, 221, 241, 0.2);
  height: 100%;
  width: 3rem;
  display: block;
  position: absolute;
  top: 0;
  left: 0;
  animation: run 1.8s ease-in-out infinite;
}
@keyframes run {
  0% {
    transform: skewX(-20deg) translateX(-5rem);
  }
  100% {
    transform: skewX(-45deg) translateX(900px);
  }
}
.drag_verify >>> .dv_handler i {
  color: #59bef2;
  font-size: 3.5em;
  vertical-align: middle;
  padding-bottom: 0.4rem;
}
</style>


