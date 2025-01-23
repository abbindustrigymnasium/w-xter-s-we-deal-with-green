import { createRouter, createWebHistory } from 'vue-router'

const router = createRouter({
  history: createWebHistory(),
  routes: [
    { path: '/', component: () => import('../views/home.vue') },
    { path: '/about', component: () => import('../views/about.vue') },
    { path: '/home', component: () => import('../views/Wexteras_Control.vue') },
  ],
})

export default router
