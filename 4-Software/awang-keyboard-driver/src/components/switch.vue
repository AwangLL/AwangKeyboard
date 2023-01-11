
<template>
  <div :class="['switch', flag ? 'active' : '']" @click="onClick()">
    <div class="icon">
      <props.activeIcon v-if="flag && props?.activeIcon" />
      <props.inactiveIcon v-if="flag == false && props?.inactiveIcon" />
    </div>
  </div>
</template>

<script setup lang="ts">
import { withDefaults, defineProps, computed } from 'vue'
import type { Component } from 'vue'

const props = withDefaults(defineProps<{
  modelValue: boolean
  width?: number
  height?: number
  inactiveIcon?: Component
  activeIcon?: Component
}>(), {
  width: 60,
  height: 30
})

const switchWidth:string = props.width - 2 + 'px'
const switchHeight:string = props.height - 2 + 'px'
const iconRadius:string = props.height - 4 + 'px'
const activeOffset:string = props.width - props.height + 'px'

const emit = defineEmits(['update:modelValue'])

const flag = computed({
  get() {
    return props.modelValue
  },
  set(value) {
    emit('update:modelValue', value)
  }
})

const onClick = () => {
  flag.value = !flag.value
}

</script>

<style scoped lang="scss">
.switch {
  border: 1px solid var(--switch-border-color);
  border-radius: 1024px;
  background-color: var(--switch-bg-color);
  height: v-bind("switchHeight");
  width: v-bind("switchWidth");
  display: flex;
  align-items: center;

  .icon {
    height: v-bind("switchHeight");
    width: v-bind("switchHeight");
    border-radius: 1024px;
    background-color: var(--html-bg-color);
    transition: all .2s linear;

    svg {
      margin-left: 1px;
      margin-top: 1px;
      width: v-bind("iconRadius");
      height: v-bind("iconRadius");
      color: var(--menu-text-color);
    }
  }
}

.active {
  .icon {
    margin-left: v-bind("activeOffset");
    transition: all .2s linear;
  }
}
</style>