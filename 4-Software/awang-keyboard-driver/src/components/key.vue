<template>
  <div :class="['key', pressed ? 'key-pressed' : '', selected ? 'key-selected' : '', disabled ? 'disabled' : 'active']">
    <div class="top" v-if="k?.labels">
      <div>{{ k.labels[0] }}</div>
      <div v-if="k?.labels.length > 1" style="margin-top: 8px;">{{ k.labels[1] }}</div>
    </div>
    <div class="top" v-else-if="k?.icon">
      <i :class="'i-ic-' + k.icon" />
    </div>
    <div class="top" v-else> </div>
  </div>
</template>


<script setup lang="ts">
import { defineProps, ref, withDefaults, computed } from 'vue'
import { getKeycode } from '@/utils'

const props = withDefaults(defineProps<{
  x?: number
  y?: number
  width?: number
  height?: number
  color?: string
  selected?: boolean
  pressed?: boolean
  keycode?: number
  disabled?: boolean
}>(), {
  x: 0,
  y: 0,
  width: 54,
  height: 54,
  color: "#FFFFFF",
  keycode: -1,
  disabled: false
})
const keyWidth: string = props.width + 'px'
const keyHeight: string = props.height + 'px'
const posX: string = props.x + 'px'
const posY: string = props.y + 'px'
const posY_: string = props.y + 2 + 'px'

const pressed = ref<boolean>(false)

const k = computed(() => getKeycode(props.keycode))

const topColor = computed(() => {
  var R = parseInt(props.color.substr(1, 2), 16);
  var G = parseInt(props.color.substr(3, 2), 16);
  var B = parseInt(props.color.substr(5, 2), 16);
  var A = props.color.length <= 7 ? 255 : parseInt(props.color.substr(7, 2), 16);

  var r = Math.floor((R * A / 255 + 255 - A))
  var g = Math.floor((G * A / 255 + 255 - A))
  var b = Math.floor((B * A / 255 + 255 - A))

  var sr = r.toString(16)
  if (sr.length == 1) sr = '0' + sr

  var sg = g.toString(16)
  if (sg.length == 1) sg = '0' + sg

  var sb = b.toString(16)
  if (sb.length == 1) sb = '0' + sb

  return '#' + sr + sg + sb
})

const backColor = computed(() => {
  var R = parseInt(props.color.substr(1, 2), 16);
  var G = parseInt(props.color.substr(3, 2), 16);
  var B = parseInt(props.color.substr(5, 2), 16);
  var A = props.color.length <= 7 ? 255 : parseInt(props.color.substr(7, 2), 16);

  var r = Math.floor((R * A / 255 + 255 - A) * 0.8)
  var g = Math.floor((G * A / 255 + 255 - A) * 0.8)
  var b = Math.floor((B * A / 255 + 255 - A) * 0.8)

  var sr = r.toString(16)
  if (sr.length == 1) sr = '0' + sr

  var sg = g.toString(16)
  if (sg.length == 1) sg = '0' + sg

  var sb = b.toString(16)
  if (sb.length == 1) sb = '0' + sb

  return '#' + sr + sg + sb
})
</script>

<style scoped lang="scss">
.key {
  box-sizing: border-box;
  width: v-bind(keyWidth);
  height: v-bind(keyHeight);
  background-color: v-bind(backColor);
  padding: 2px 6px 9px 6px;
  border-radius: 3px;
  opacity: 0.8;
  transform: translate(v-bind(posX), v-bind(posY));
  transition: all .2s ease-out 0s;

  .top {
    box-sizing: border-box;
    width: 100%;
    height: 100%;
    padding-left: 4px;
    overflow: hidden;
    display: flex;
    flex-direction: column;
    justify-content: center;
    font-size: small;
    font-weight: bold;
    color: #000;
    border-radius: 3px;
    background-color: v-bind(topColor);

    i {
      position: absolute;
    }
  }
}

.active.key {
  opacity: 0.8;
}

.active.key:hover {
  transform: translate(v-bind(posX), v-bind(posY_));
  transition: transform .2s ease-out 0s;
  opacity: 0.4;
}

.active.key.key-selected {
  transition: all .2s ease-out 0s;
  opacity: 1;
}
</style>