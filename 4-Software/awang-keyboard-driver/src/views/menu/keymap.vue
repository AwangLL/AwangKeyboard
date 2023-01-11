<template>
  <div class="container">
    <div class="main">
      <aw-key-group :keys="config.keys" :width="config.size.width" :height="config.size.height"
        v-model="keyIndex"></aw-key-group>
    </div>
    <hr />
    <div class="footer">
      <div class="ft-left">
        <!-- reset -->
        <div :class="['menu', activeMenu == MenuType.reset ? 'menu-active' : '']" @click="switchMenu(MenuType.reset)">
          <icon-reset /> <span>reset</span>
        </div>
        <!-- mouse -->
        <div :class="['menu', activeMenu == MenuType.mouse ? 'menu-active' : '']" @click="switchMenu(MenuType.mouse)">
          <icon-mouse /> <span>mouse</span>
        </div>
        <!-- keyboard -->
        <div :class="['menu', activeMenu == MenuType.keyboard ? 'menu-active' : '']"
          @click="switchMenu(MenuType.keyboard)">
          <icon-keyboard /> <span>keyboard</span>
        </div>
        <!-- media -->
        <div :class="['menu', activeMenu == MenuType.media ? 'menu-active' : '']" @click="switchMenu(MenuType.media)">
          <icon-media /> <span>media</span>
        </div>
        <!-- lnk -->
        <div :class="['menu', activeMenu == MenuType.lnk ? 'menu-active' : '']" @click="switchMenu(MenuType.lnk)">
          <icon-lnk /> <span>windows lnk</span>
        </div>
        <!-- macro -->
        <div :class="['menu', activeMenu == MenuType.macro ? 'menu-active' : '']" @click="switchMenu(MenuType.macro)">
          <icon-macro /> <span>macro</span>
        </div>
      </div>
      <div class="ft-middle">
        <div class="title">{{ title }}</div>
        <div class="key"> {{ keyIndex != -1 ? config.keys[keyIndex].labels[0] : '' }}</div>
      </div>
      <div class="ft-right">

      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import AwKeyGroup from '@/components/key-group.vue'
import IconReset from '@/components/icons/reset.vue'
import IconMouse from '@/components/icons/mouse.vue'
import IconKeyboard from '@/components/icons/keyboard.vue'
import IconMedia from '@/components/icons/media.vue'
import IconMacro from "@/components/icons/macro.vue";
import IconLnk from '@/components/icons/lnk.vue'
import { useConfigStore } from '@/stores';
import { ref } from 'vue';

enum MenuType {
  reset,
  mouse,
  keyboard,
  media,
  lnk,
  macro
}

const config = useConfigStore()
const keyIndex = ref<number>(-1)
const title = ref<string>('default key')
const activeMenu = ref<MenuType>(MenuType.reset)

function switchMenu(type: MenuType) {
  activeMenu.value = type
  switch (type) {
    case MenuType.reset:
      title.value = 'default key'
      break;
    case MenuType.mouse:
      title.value = 'mouse key'
      break;
    case MenuType.keyboard:
      title.value = 'keyboard key'
      break;
    case MenuType.media:
      title.value = 'media key'
      break;
    case MenuType.lnk:
      title.value = 'windows lnk key'
      break;
    case MenuType.macro:
      title.value = 'macro key'
      break;
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
    height: 300px;
    display: flex;

    .ft-left {
      width: 200px;

      .menu {
        height: 40px;
        width: 90%;
        margin: 5px;
        border-radius: 5px;
        position: relative;
        transition: 0.3s;
        color: var(--menu-text-color);
      }

      .menu svg {
        font-size: 32px;
        position: absolute;
        margin: 4px 0 4px 13px;
      }

      .menu span {
        position: relative;
        top: 5px;
        left: 55px;
        font: 500 20px "";
      }

      .menu-active {
        color: var(--menu-active-text-color);
      }

      .menu:hover {
        background-color: var(--menu-hover-bg-color);
      }
    }

    .ft-middle {
      width: 200px;
      padding: 10px;

      .title {
        font: 500 20px "";
        color: var(--menu-text-color);
      }

      .key {
        margin-top: 20px;
        border: 2px solid var(--menu-text-color);
        border-radius: 5px;
        width: 100px;
        height: 30px;
        padding: 5px;
        color: var(--menu-text-color);
        font: 500 20px '';
      }
    }

    .ft-right {
      background-color: red;
      flex: 1;
    }
  }
}
</style>