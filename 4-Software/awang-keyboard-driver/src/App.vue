<template>
  <RouterView></RouterView>
</template>

<script setup lang="ts">
import { RouterView } from "vue-router";
</script>

<style>
html,
body,
#app {
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
