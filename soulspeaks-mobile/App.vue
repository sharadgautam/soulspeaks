<template>
  <view style="flex: 1; justify-content: center; align-items: center; background: #f4f7fa;">
    <text style="font-size: 28px; font-weight: bold; margin-bottom: 20px; color: #2980b9;">SoulSpeaks Mobile</text>
    <text style="font-size: 18px; margin-bottom: 40px; color: #555;">Welcome to your Vue Native app!</text>
    <button @press="screen = 'login'" style="margin-bottom: 10px;">Login</button>
    <button @press="screen = 'qr'" style="margin-bottom: 10px;">QR Scan</button>
    <button @press="screen = 'home'">Home</button>
    <view v-if="screen === 'login'" style="margin-top: 30px; width: 90%;">
      <text style="font-size: 20px; margin-bottom: 10px;">Login Screen (placeholder)</text>
      <!-- Add login form here -->
    </view>
    <view v-if="screen === 'qr'" style="margin-top: 30px; width: 90%;">
      <text style="font-size: 20px; margin-bottom: 10px;">QR Scan Screen (placeholder)</text>
      <!-- Add QR scanner here -->
    </view>
    <view v-if="screen === 'home'" style="margin-top: 30px; width: 90%;">
      <text style="font-size: 20px; margin-bottom: 10px;">Home Screen (placeholder)</text>
      <!-- Add home/dashboard here -->
    </view>
  </view>
</template>

<script>
export default {
  data() {
    return {
      screen: null
    };
  }
};
</script>

<style>
.container {
  background-color: white;
  align-items: center;
  justify-content: center;
  flex: 1;
}
.text-color-primary {
  color: blue;
}
</style>
