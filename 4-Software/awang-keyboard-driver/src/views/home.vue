<template>
  <div class="container">
    <div class="aside">
      <!-- logo -->
      <img src="/logo.ico" class="logo" alt="" />
      <!-- menu -->
      <!-- keymap -->
      <div
        :class="[
          'menu',
          activeMenuIndex == MenuType.keymap ? 'menu-active' : '',
        ]"
        @click="switchMenu(MenuType.keymap)"
      >
        <icon-keymap /><span>keymap</span>
      </div>
      <!-- light -->
      <div
        :class="[
          'menu',
          activeMenuIndex == MenuType.light ? 'menu-active' : '',
        ]"
        @click="switchMenu(MenuType.light)"
      >
        <icon-light /><span>light</span>
      </div>
      <!-- macro -->
      <div
        :class="[
          'menu',
          activeMenuIndex == MenuType.macro ? 'menu-active' : '',
        ]"
        @click="switchMenu(MenuType.macro)"
      >
        <icon-macro /><span>macro</span>
      </div>
      <!-- setting -->
      <div
        :class="[
          'menu',
          activeMenuIndex == MenuType.setting ? 'menu-active' : '',
        ]"
        @click="switchMenu(MenuType.setting)"
      >
        <icon-setting /><span>setting</span>
      </div>
    </div>
    <div class="main">
      <router-view></router-view>
    </div>
  </div>
</template>

<script setup lang="ts">
import IconKeymap from "@/components/icons/keymap.vue";
import IconLight from "@/components/icons/light.vue";
import IconMacro from "@/components/icons/macro.vue";
import IconSetting from "@/components/icons/setting.vue";
// import IconSave from '@/components/icons/save.vue'
import { ref } from "vue";

enum MenuType {
  keymap,
  light,
  macro,
  setting,
}

const activeMenuIndex = ref<MenuType>(MenuType.keymap);

const switchMenu = (type: MenuType) => {
  activeMenuIndex.value = type;
};
</script>

<style lang="scss">
.container {
  height: 100%;
  overflow: hidden;
  display: flex;

  .aside {
    width: 100px;
    height: 100%;
    display: flex;
    align-items: center;
    flex-direction: column;
    background-color: var(--menu-bg-color);
    // border-radius: 10px;
    transition: 0.3s;
    overflow: hidden;

    .logo {
      margin-top: 10px;
      margin-bottom: 30px;
      width: 64px;
      height: 64px;
    }

    .menu {
      height: 70px;
      width: 85%;
      margin: 5px;
      border-radius: 5px;
      position: relative;
      transition: 0.3s;
      color: var(--menu-text-color);
    }

    .menu svg {
      font-size: 60px;
      position: absolute;
      margin: 5px 0 5px 13px;
    }

    .menu span {
      position: relative;
      top: 20px;
      left: 80px;
      font: 500 20px "";
      opacity: 0;
      transition: 0.1s;
    }

    .menu-active {
      color: var(--menu-active-text-color);
    }

    .menu:hover {
      background-color: var(--menu-hover-bg-color);
      // color:var(--menu-hover-text-color);
    }
  }

  .aside:hover {
    width: 270px;
    span {
      opacity: 1;
    }
  }

  .main {
    flex: 1;
    height: 100%;
  }
}

/* .menu:nth-child(1)::before,
.menu:nth-child(5)::before {
  content: '';
  display: block;
  width: 100%;
  height: 2px;
  background-color: rgb(55, 65, 81);
  position: absolute;
  bottom: -10px;
} */
</style>
