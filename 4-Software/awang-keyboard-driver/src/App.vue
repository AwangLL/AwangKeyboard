<template>
  <i v-if="false" 
    i-ic-outline-volume-off 
    i-ic-outline-volume-up 
    i-ic-outline-volume-down
    i-ic-round-play-circle-outline
    i-ic-outline-skip-previous
    i-ic-outline-skip-next
    i-ic-outline-calculate
    i-ic-outline-playlist-add-check
    i-ic-outline-email
    i-ic-outline-desktop-windows
    i-ic-outline-folder 
    i-ic-outline-dark-mode
    i-ic-outline-light-mode />
  <RouterView></RouterView>
</template>

<script setup lang="ts">
import { RouterView } from "vue-router";
</script>

<style>
html, body, #app {
  height: 100%;
}
#app {
  overflow: hidden;
  min-width: 1024px;
  min-height: 768px;
  max-width: 100vw;
  max-height: 100vh;
  background-color: var(--html-bg-color);
}
</style>
