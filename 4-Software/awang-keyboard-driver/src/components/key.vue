<template>
  <div :class="['key', pressed ? 'key-pressed' : '', selected ? 'key-selected' : '']">
    <div class="top" v-if="props.k.labels.length == 1">
      <div>{{ props.k.labels[0] }}</div>
    </div>
    <div class="top" v-else>
      <div>{{ props.k.labels[0] }}</div>
      <div style="margin-top: 8px;">{{ props.k.labels[6] }}</div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { defineProps, ref } from 'vue'
import type { Key } from '@ijprest/kle-serial';

const props = defineProps<{ 
  k: Key,
  selected?: boolean,
  pressed?: boolean
}>()
const keyWidth: string = props.k.width - 12 + 'px'
const keyHeight: string = props.k.height - 11 + 'px'
const posX: string = props.k.x + 'px'
const posY: string = props.k.y + 'px'
const posY_: string = props.k.y + 2 + 'px'
const keyColor = props.k.color + "BB"
const topColor = props.k.color

const pressed = ref<boolean>(false)

</script>

<style scoped lang="scss">
.key {
  width: v-bind(keyWidth);
  height: v-bind(keyHeight);
  background-color: v-bind(keyColor);
  padding: 2px 6px 9px 6px;
  transform: translate(v-bind(posX), v-bind(posY));
  border-radius: 3px;
  opacity: 0.7;
  transition: all .2s ease-out 0s;

  .top {
    box-sizing: border-box;
    width: v-bind(keyWidth);
    height: v-bind(keyHeight);
    padding: 4px;
    overflow: hidden;
    display: flex;
    flex-direction: column;
    justify-content: center;
    font-size: small;
    font-weight: bold;
    color: #000;
    border-radius: 3px;
    background-color: v-bind(topColor);
  }
}

.key-pressed {
  transform: translate(v-bind(posX), v-bind(posY_));
  opacity: 0.4;
  transition: all .2s ease-out 0s;
}

.key:hover {
  opacity: 1;
  transition: all .2s ease-out 0s;
}
.key-selected {
  opacity: 1;
  transition: all .2s ease-out 0s;
}
</style>