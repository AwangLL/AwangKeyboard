import { createRouter, createWebHistory } from "vue-router";

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: "/",
      name: "home",
      component: () => import("@/views/home.vue"),
      children: [
        {
          path: "/keymap",
          name: "keymap",
          component: () => import("@/views/menu/keymap.vue")
        },
        {
          path: "/light",
          name: "light",
          component: () => import("@/views/menu/light.vue")
        },
        {
          path: "/macro",
          name: "macro",
          component: () => import("@/views/menu/macro.vue")
        },
        {
          path: "/setting",
          name: "setting",
          component: () => import("@/views/menu/setting.vue")
        }
      ]
    },

  ],
});

export default router;
