<template>
  <div class="container">
    <div class="main">
      <aw-key-group :keys="config.keys" :width="config.size.width" :height="config.size.height" :color="colors"
        v-model="keyIndex" disabled></aw-key-group>
    </div>
    <hr />
    <div class="footer"> </div>
  </div>

</template>

<script setup lang="ts">
import AwKeyGroup from '@/components/key-group.vue'
import { useConfigStore } from '@/stores';
import { ref } from 'vue';

const config = useConfigStore()
const keyIndex = ref<number>(-1)
const colors = ref<Array<string>>([])

</script>

<style scoped lang="scss">
.container {
  height: 100%;
  display: flex;
  flex-direction: column;

  .main {
    flex: 1;
    width: 100%;
    overflow: hidden;
    display: flex;
    align-items: center;
    justify-content: center;
  }

  .footer {
    width: 100%;
    height: 320px;
    display: flex;
  }
}
</style>