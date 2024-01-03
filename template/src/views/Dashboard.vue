<script setup>
import Storage from '../utils/Storage';

const userData = Storage.get('INFO_ACCOUNT', null);
const userName = userData.name;
</script>

<template>
  <div class="card">
      <h3>Hello {{ userName }}</h3>
      <p class="bg-primary border-round p-3">This is link my project  <a href="https://github.com/khanhkhanhlele/smarthome_IOT" class="font-bold bg-primary hover:underline">github</a> .</p>
      <h5>Demo video</h5>

      <div class="video-container">
          <iframe className="video" width="560" height="315" src="https://www.youtube.com/embed/AHeSjJFR3ZE" frameborder="0" allowfullscreen></iframe>
      </div>
  </div>
</template>

<style lang="scss" scoped>
@media screen and (max-width: 991px) {
  .video-container {
      position: relative;
      width: 100%;
      height: 0;
      padding-bottom: 56.25%;

      iframe {
          position: absolute;
          top: 0;
          left: 0;
          width: 100%;
          height: 100%;
      }
  }
}
</style>
