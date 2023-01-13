import { defineStore } from "pinia";
import type { Key } from "@ijprest/kle-serial";
import { Serial } from "@ijprest/kle-serial"
import { keycode, config } from "@/utils";

export const useConfigStore = defineStore('config', {
  state: () => {
    return {default: config, keymap: JSON.parse(JSON.stringify(config.keymap)), fn: JSON.parse(JSON.stringify(config.fn))}
  },
  getters: {
    keys(): Key[] {
      const keySize: number = 56
      var _keys = Serial.deserialize(this.default.layout).keys
      _keys.forEach(key => {
        key.width = key.width * keySize - 2
        key.height = keySize * key.height - 2
        key.x *= keySize
        key.y *= keySize
      })
      return _keys
    },
    size(): { width: string, height: string } {
      var width: number = 0
      var height: number = 0
      this.keys.forEach(key => {
        let tw = key.width + key.x
        let th = key.height + key.y
        width = width > tw ? width : tw
        height = height > th ? height : th
      });
      return {
        width: width + 'px',
        height: height + 'px'
      }
    }
  },
  actions: {
  }
})