<template>
  <div class="container">
    <aw-switch v-model="isDark" inactive-icon="outline-light-mode" active-icon="outline-dark-mode" @click="onDarkChange"/>
  </div>
</template>

<script setup lang="ts">
import AwSwitch from '@/components/switch.vue'
import { useDark } from '@vueuse/core'
import { ElMessage } from 'element-plus'

const isDark = useDark()

function onDarkChange() {
  if(isDark.value) {
    ElMessage.success("切换至深色模式")
  } else {
    ElMessage.success("切换至浅色模式")
  }
}
</script>

<style scoped>
.container {
  margin: 20px;
  height: 100%;
}
</style>
