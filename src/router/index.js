import { createRouter, createWebHistory } from 'vue-router'
import { auth } from '../boot/firebase'

const router = createRouter({
  history: createWebHistory(),
  routes: [
    { path: '/', component: () => import('../views/home.vue') },
    { path: '/about', component: () => import('../views/about.vue') },
    {
      path: '/controls',
      component: () => import('../views/Wexteras_Control.vue'),
      meta: { requiresAuth: true },
    },
    {
      path: '/login',
      component: () => import('../views/login.vue'),
    },
  ],
})

router.beforeEach((to, from, next) => {
  const requiresAuth = to.matched.some((record) => record.meta.requiresAuth)

  auth.onAuthStateChanged((user) => {
    if (requiresAuth && !user) {
      next({ path: '/login', query: { redirect: to.fullPath } })
    } else {
      next()
    }
  })
})

export default router
