<template>
  <div class="container">
    <div class="aside">
      <!-- logo -->
      <img src="/logo.ico" class="logo" alt="" />
      <span class="name">{{ config.brief }}</span>
      <!-- menu -->
      <!-- keymap -->
      <div :class="['menu', activeMenu == MenuType.keymap ? 'menu-active' : '',]" @click="switchMenu(MenuType.keymap)">
        <icon-keymap /><span>keymap</span>
      </div>
      <!-- light -->
      <div :class="['menu', activeMenu == MenuType.light ? 'menu-active' : '',]" @click="switchMenu(MenuType.light)">
        <icon-light /><span>light</span>
      </div>
      <!-- macro -->
      <div :class="['menu', activeMenu == MenuType.macro ? 'menu-active' : '',]" @click="switchMenu(MenuType.macro)">
        <icon-macro /><span>macro</span>
      </div>
      <!-- setting -->
      <div :class="['menu', activeMenu == MenuType.setting ? 'menu-active' : '',]"
        @click="switchMenu(MenuType.setting)">
        <icon-setting /><span>setting</span>
      </div>
      <div style="flex:1"></div>
      <!-- open file -->
      <label class="menu">
        <icon-open-file /> <span>import</span>
        <input type="file" hidden @change="importFile">
      </label>
    </div>
    <div class="main">
      <router-view v-slot="{ Component }">
        <transition enter-active-class="animate__animated animate__bounceInDown">
          <component :is="Component" />
        </transition>
      </router-view>
    </div>
  </div>
</template>

<script setup lang="ts">
import IconKeymap from "@/components/icons/keyboard.vue";
import IconLight from "@/components/icons/light.vue";
import IconMacro from "@/components/icons/macro.vue";
import IconSetting from "@/components/icons/setting.vue";
import IconOpenFile from "@/components/icons/open-file.vue"
import { useConfigStore } from "@/stores";
import { useRouter } from "vue-router";
import { useDark } from "@vueuse/core";
import { ref } from "vue";
import { ElMessage } from "element-plus";

enum MenuType {
  none,
  keymap,
  light,
  macro,
  setting,
}

const router = useRouter()
const config = useConfigStore()
const activeMenu = ref<MenuType>(MenuType.none);

function switchMenu(type: MenuType) {
  activeMenu.value = type;
  switch (type) {
    case MenuType.keymap:
      router.push({
        name: "keymap"
      })
      break;
    case MenuType.light:
      router.push({
        name: "light"
      })
      break;
    case MenuType.macro:
      router.replace({
        name: "macro"
      })
      break;
    case MenuType.setting:
      router.replace({
        name: "setting"
      })
      break;
  }
};
function importFile(e: any) {
  var fr = new FileReader()
  var file: File = e.target.files[0]
  if (file.type != "application/json") {
    ElMessage.error("配置文件需要是json类型")
    return
  }
  fr.onload = function () {
    try {
      var json = JSON.parse(this.result as string)
      config.$state = json
      ElMessage.success("成功导入配置文件")
    } catch {
      ElMessage.error("配置文件不符合要求")
    }
  };
  fr.readAsText(file)
}

// setup
// switch menu
switchMenu(MenuType.keymap)
// select theme
useDark()


</script>

<style lang="scss" scoped>
.container {
  height: 100%;
  display: flex;

  .aside {
    width: 100px;
    height: 100%;
    display: flex;
    align-items: center;
    flex-direction: column;
    background-color: var(--menu-bg-color);
    transition: 0.3s;
    overflow: hidden;

    .logo {
      margin-top: 10px;
      margin-bottom: 10px;
      width: 64px;
      height: 64px;
    }

    .name {
      margin-bottom: 10px;
      color: var(--menu-text-color);
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
    }
  }

  .aside:hover {
    width: 270px;
    transition: 0.3s;

    span {
      opacity: 1;
      transition: .3s ease-out;
    }
  }

  .main {
    flex: 1;
    height: 100%;
    overflow: hidden;
  }
}
</style>
