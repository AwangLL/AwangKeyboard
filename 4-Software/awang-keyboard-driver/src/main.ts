import { createApp, toRaw } from "vue";
import { createPinia, type PiniaPluginContext } from "pinia";

import App from "./App.vue";
import router from "./router";

const app = createApp(App);

// global css
import "@/assets/reset.css";
import "@/assets/base.css";
import "animate.css"

// element pulus css
import 'element-plus/es/components/message/style/index'
import 'element-plus/es/components/select/style/index'

// pinia
const __piniaKey = '__PINIAKEY__'

type Options = {
  key?: string
}

const setStorage = (key: string, value: any): void => {
  localStorage.setItem(key, JSON.stringify(value))
}
const getStorage = (key: string) => {
  return (localStorage.getItem(key) ? JSON.parse(localStorage.getItem(key) as string) : {})
}
const piniaPlugin = (options: Options) => {
  return (context: PiniaPluginContext) => {
    const { store } = context;
    const data = getStorage(`${options?.key ?? __piniaKey}-${store.$id}`)
    store.$subscribe(() => {
      setStorage(`${options?.key ?? __piniaKey}-${store.$id}`, toRaw(store.$state));
    })
    return {
      ...data
    }
  }
}

const store = createPinia()
store.use(piniaPlugin({
  key: 'pinia'
}))

app.use(store);
app.use(router);

app.mount("#app");
