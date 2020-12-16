<template>
  <div class="tokenInput">
    <input
      class="token-input"
      type="text"
      v-model="token"
      placeholder="输入Token"
    />
    <button class="token-btn" v-on:click="submit">提交</button>
  </div>
</template>

<script>
export default {
  props: ["initToken"],
  data() {
    console.log(this.initToken)
    return {
      token: "",
      // hasToken: this.initToken != null,
      submit: () => {
        localStorage.setItem("token", this.token);
        // this.hasToken = true;
        this.$emit('get-token')
      },
    };
  }
};
</script>

<style scoped>
.token-input {
  display: block;
  outline-style: none;
  border: 1px solid #ccc;
  border-radius: 3px;
  padding: 1.5rem 14px;
  width: 12rem;
  font-size: 1.2rem;
  font-weight: 700;
  margin: 5rem auto auto auto;
}
.token-btn {
  color: #7fc6e8;
  border-radius: 0.2px;
  width: 8rem;
  padding: 0.5rem 2rem;
  border: none;
  box-shadow: 0 8px 16px 0 rgba(0, 0, 0, 0.2);
  outline: none;
  font-size: 1.2rem;
  background-color: #333b4a;
  margin-top: 2rem;
}
.token-btn:active {
  background-color: #53617a;
}
</style>