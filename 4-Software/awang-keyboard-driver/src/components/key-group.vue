<template>
  <div class="key-group">
    <div class="key-box" v-for="(item, index) in props.keys">
      <aw-key :x="item.x" :y="item.y" :width="item.width" :height="item.height" :keycode="props.keymap[index]" :color="props.color[index]"
      :selected="select[index]" :pressed="select[index]" @click="selectKey(index as number)" :disabled="props.disabled"/>
    </div>
  </div>
</template>

<script setup lang="ts">
import AwKey from '@/components/key.vue'
import { defineProps, defineEmits, ref, computed, withDefaults } from 'vue';
import type { Key } from "@ijprest/kle-serial"

const props = withDefaults(defineProps<{
  modelValue: number // selected key index
  keys: Array<Key>
  width: string
  height: string
  keymap?: Array<number>
  color?: Array<string>
  disabled?: boolean
}>(), {
  keymap: () => [],
  color: () => [],
  disabled: false
})
const emit = defineEmits(['update:modelValue'])

const selectedIndex = computed({
  get() {
    return props.modelValue
  },
  set(value) {
    emit('update:modelValue', value)
  }
})

const select = ref<Array<boolean>>(new Array<boolean>(props.keys.length));
const press = ref<Array<boolean>>(new Array<boolean>(props.keys.length));

select.value.fill(false)
press.value.fill(false)

function selectKey(index: number):void {
  if(selectedIndex.value != -1) {
    select.value[selectedIndex.value] = false
  }
  select.value[index] = true
  selectedIndex.value = index
}
</script>

<style scoped lang="scss">
.key-group {
  width: v-bind("props.width");
  height: v-bind("props.height");
  padding: 5px;
  border: 3px solid var(--switch-border-color);
  border-radius: 6px;
  position: relative;
  transform: scale(1, 1);
  .key-box {
    height: 0px;
    width: 0px;
    transform: rotate3d(0, 0, 1, 0deg);
    transform-origin: 0px 0px;
    position: absolute;
  }
}
</style>