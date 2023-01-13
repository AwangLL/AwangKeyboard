import keycodeJson from '@/assets/keycode.json'
import configJson from '@/assets/config.json'

export enum MenuType {
  none,
  keymap,
  light,
  macro,
  setting,
}

export enum KeycodeType {
  reset,
  keyboard,
  keyboardFn,
  keyboardCtrl,
  keyboardPad,
  mouse,
  systemMedia,
  systemApp,
  macro,
  device
}

export declare class Config {
  brief: string;
  name: string;
  version: string;
  layout: Array<any>;
  keymap: Array<number>;
  fn: Array<number>;
}

export declare class Keycode {
  keycode: number;
  labels?: string[];
  icon?: string;
  info: string;
  type: KeycodeType
}

export const keycode: Array<Keycode> = keycodeJson
export const config: Config = configJson

export function getKeycode(kc: number): Keycode | undefined {
  var l = 0, r = keycode.length - 1, m = 0
  while (l <= r) {
    m = Math.floor((l + r) / 2)

    if (keycode[m].keycode == kc) {
      return keycode[m]
    } else {
      if (keycode[m].keycode > kc) {
        r = m - 1
      } else {
        l = m + 1
      }
    }
  }
  return undefined
}

export function getKeycodeByType(types: Array<KeycodeType>) {
  return keycode.filter(
    (data) => types.includes(data.type)
  )
}