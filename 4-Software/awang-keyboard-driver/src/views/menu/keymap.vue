<template>
  <div class="container">
    <div class="main">
      <aw-key-group :keys="config.keys" :width="config.size.width" :height="config.size.height"
        :keymap="isFn ? config.fn : config.keymap" v-model="keyIndex"/>
    </div>
    <hr />
    <div class="footer">
      <div class="ft-left">
        <div class="fn-switch">
          common<aw-switch v-model="isFn"></aw-switch>fn
        </div>
        <!-- keyboard -->
        <div :class="['menu', activeKeycodeType == KeycodeType.keyboard ? 'menu-active' : '']"
          @click="switchKeycodeType(KeycodeType.keyboard)">
          <i class="i-ic-outline-keyboard" /> <span>keyboard</span>
        </div>
        <!-- mouse -->
        <div :class="['menu', activeKeycodeType == KeycodeType.mouse ? 'menu-active' : '']"
          @click="switchKeycodeType(KeycodeType.mouse)">
          <i class="i-ic-outline-mouse" /> <span>mouse</span>
        </div>
        <!-- system fn -->
        <div :class="['menu', activeKeycodeType == KeycodeType.systemMedia ? 'menu-active' : '']"
          @click="switchKeycodeType(KeycodeType.systemMedia)">
          <i class="i-ic-outline-desktop-windows" /> <span>system fn</span>
        </div>
        <!-- macro -->
        <div :class="['menu', activeKeycodeType == KeycodeType.macro ? 'menu-active' : '']"
          @click="switchKeycodeType(KeycodeType.macro)">
          <i class="i-ic-outline-fiber-smart-record" /> <span>macro</span>
        </div>
        <!-- device -->
        <div :class="['menu', activeKeycodeType == KeycodeType.device ? 'menu-active' : '']"
          @click="switchKeycodeType(KeycodeType.device)">
          <i class="i-ic-outline-miscellaneous-services" /> <span>device fn</span>
        </div>

        <div style="flex: 1;" />
        <div class="bottom-function">
          <el-tooltip class="box-item" :effect="isDark ? 'dark' : 'light'" content="reset" placement="top">
            <i class="i-ic-outline-lock-reset" @click="resetKey" />
          </el-tooltip>
          <el-tooltip class="box-item" :effect="isDark ? 'dark' : 'light'" content="disable" placement="top">
            <i class="i-ic-outline-do-disturb" @click="disableKey" />
          </el-tooltip>
          <el-tooltip class="box-item" :effect="isDark ? 'dark' : 'light'" content="save" placement="top">
            <i class="i-ic-outline-save" @click="saveKey" />
          </el-tooltip>
        </div>
      </div>
      <div class="ft-right">
        <div style="overflow: scroll;">
          <table>
            <!-- <tbody> -->
            <transition-group tag="tbody" enter-active-class="animate__animated animate__fadeInRight">
              <tr v-for="item in filterKeycode" @click="selectKeycode(item)" :key="item.keycode">
                <td class="cell-keycode">{{ item.keycode }}</td>
                <td class="cell-icon" v-if="item.icon" :class="'i-ic-' + item.icon"> </td>
                <td v-if="item.icon" style="width: 120px;"></td>
                <td class="cell-labels" v-if="item.labels"> {{ item.labels[0] }} </td>
                <td> {{ item.info }}</td>
                <!-- <td class="cell-type"> {{ item.type }} </td> -->
              </tr>
            </transition-group>
            <!-- </tbody> -->
          </table>
        </div>
        <hr />
        <div class="info">
          {{ selectedKey?.info }}
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import AwKeyGroup from '@/components/key-group.vue'
import AwSwitch from '@/components/switch.vue'
import { useConfigStore } from '@/stores';
import { ref, watch, computed } from 'vue';
import { KeycodeType, type Keycode, getKeycode, getKeycodeByType } from '@/utils'
import { useDark } from '@vueuse/core';
import { ElMessage } from 'element-plus';

const config = useConfigStore()
const isDark = useDark()
const keyIndex = ref<number>(-1)
const activeKeycodeType = ref<KeycodeType>(KeycodeType.reset)
const isFn = ref<boolean>(false)

const selectedKey = computed(() => getKeycode(isFn.value ? config.fn[keyIndex.value] : config.keymap[keyIndex.value]))
const filterKeycode = computed(() => {
  switch (activeKeycodeType.value) {
    case KeycodeType.keyboard:
      return getKeycodeByType([KeycodeType.keyboard, KeycodeType.keyboardFn, KeycodeType.keyboardCtrl, KeycodeType.keyboardPad])
    case KeycodeType.mouse:
      return getKeycodeByType([KeycodeType.mouse])
    case KeycodeType.systemMedia:
      return getKeycodeByType([KeycodeType.systemMedia, KeycodeType.systemApp])
    case KeycodeType.macro:
      return getKeycodeByType([KeycodeType.macro])
    case KeycodeType.device:
      return getKeycodeByType([KeycodeType.device])
  }
})

switchKeycodeType(KeycodeType.keyboard)

watch(keyIndex, () => {
  // if (keyIndex.value == -1) return
  switch (selectedKey.value!.type) {
    case KeycodeType.keyboard:
    case KeycodeType.keyboardFn:
    case KeycodeType.keyboardCtrl:
    case KeycodeType.keyboardPad:
      switchKeycodeType(KeycodeType.keyboard)
      break;
    case KeycodeType.mouse:
      switchKeycodeType(KeycodeType.mouse)
      break;
    case KeycodeType.systemMedia:
    case KeycodeType.systemApp:
      switchKeycodeType(KeycodeType.systemMedia)
      break;
    case KeycodeType.macro:
      switchKeycodeType(KeycodeType.macro)
      break;
    case KeycodeType.device:
      switchKeycodeType(KeycodeType.device)
      break;
  }
})

function switchKeycodeType(type: KeycodeType) {
  if (activeKeycodeType.value == type) return
  activeKeycodeType.value = type
}

function resetKey() {
  if (keyIndex.value == -1) {
    ElMessage.error("请选中要修改的按键")
    return
  }
  if (isFn.value) {
    config.fn[keyIndex.value] = config.default.fn[keyIndex.value]
  } else {
    config.keymap[keyIndex.value] = config.default.keymap[keyIndex.value]
  }
}

function disableKey() {
  if (keyIndex.value == -1) {
    ElMessage.error("请选中要修改的按键")
    return
  }
  if (isFn.value) {
    config.fn[keyIndex.value] = 0
  } else {
    config.keymap[keyIndex.value] = 0
  }
}

function saveKey() {
  ElMessage.success("保存按键配置成功")
}

function selectKeycode(keycode: Keycode) {
  if (keyIndex.value == -1) {
    ElMessage.error("请选中要修改的按键")
    return
  }
  if (isFn.value) {
    config.fn[keyIndex.value] = keycode.keycode
  } else {
    config.keymap[keyIndex.value] = keycode.keycode
  }
}

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

    .ft-left {
      width: 200px;
      display: flex;
      flex-direction: column;
      align-items: center;

      .fn-switch {
        margin-top: 2px;
        display: flex;
        flex-direction: row;
        justify-content: space-between;
        width: 85%;
        align-items: center;
        color: var(--menu-text-color);
      }

      .menu {
        height: 40px;
        width: 95%;
        margin: 3px;
        border-radius: 5px;
        transition: 0.3s;
        color: var(--menu-text-color);
        display: flex;
        align-items: center;

        i {
          font-size: 32px;
          margin-left: 10px;
        }

        span {
          margin-left: 10px;
          font-size: 20px;
        }
      }

      .menu-active {
        color: var(--menu-active-text-color);
      }

      .menu:hover {
        background-color: var(--menu-hover-bg-color);
      }

      .bottom-function {
        display: flex;
        flex-direction: row;
        width: 95%;
        height: 40px;
        margin-bottom: 3px;
        justify-content: space-between;

        i {
          flex: 1;
          color: var(--menu-text-color);
          font-size: 32px;
        }

        i:hover {
          color: var(--menu-active-text-color);
        }
      }
    }

    .ft-right {
      flex: 1;
      display: flex;
      flex-direction: column;

      table {
        width: 100%;
        border-collapse: collapse;
        color: var(--menu-text-color);
      }

      tr {
        height: 30px;
        line-height: 30px;
        text-align: left;
        border-width: 1px 0;
        border-style: solid;
        border-color: var(--menu-text-color);

        .cell-keycode {
          padding-left: 10px;
          width: 100px;
        }

        .cell-labels {
          width: 150px;
        }

        .cell-icon {
          width: 30px;
        }

        .cell-type {
          width: 100px;
        }
      }

      tr:first-child,
      tr:last-child {
        border-width: 0;
      }

      tr:hover {
        background-color: var(--menu-bg-color);
      }

      .info {
        min-height: 30px;
        margin: 0 5px 5px 5px;
        color: var(--menu-text-color);
        line-height: 2em;
      }
    }
  }


}
</style>