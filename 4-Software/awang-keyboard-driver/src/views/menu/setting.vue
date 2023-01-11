<template>
  <div class="container">
    <aw-switch v-model="isDark" :inactive-icon="IconSun" :active-icon="IconMoon" @click="onDarkChange"/>
  </div>
</template>

<script setup lang="ts">
import AwSwitch from '@/components/switch.vue'
import IconSun from '@/components/icons/sun.vue'
import IconMoon from '@/components/icons/moon.vue'
import { getCurrentInstance } from 'vue'
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
