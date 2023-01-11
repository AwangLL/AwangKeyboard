import { defineStore } from "pinia";
import type { Key } from "@ijprest/kle-serial";
import { Serial } from "@ijprest/kle-serial"
import defalutConfig from '@/assets/config.json'

export const useConfigStore = defineStore('config', {
  state: () => {
    return defalutConfig
  },
  getters: {
    keys(): Key[] {
      const keySize:number = 56
      var _keys = Serial.deserialize(this.layout).keys
      var x:number = _keys[0].x
      var y:number = _keys[0].y
      var cnt:number = 0
      _keys.forEach(key => {
        key.width = key.width * keySize - 2
        key.height = keySize * key.height - 2
        key.x *= keySize
        key.y *= keySize
      })
      return _keys
    },
    size(): {width: string, height: string} {
      const keySize:number = 54
      var width:number = 0
      var height:number = 0
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