import Vue from 'vue'
import VueRouter, { RouteConfig } from 'vue-router'
import Home from '../views/Home.vue'
import Remote from '../views/Remote.vue'
import Wifi from '../views/Wifi.vue'

Vue.use(VueRouter)

const routes: Array<RouteConfig> = [
  {
    path: '/',
    name: 'home',
    component: Home
  },
  {
    path: '/remote',
    name: 'remote',
    component: Remote
  },
  {
    path: '/wifi',
    name: 'wifi',
    component: Wifi
  }
]

const router = new VueRouter({
  routes
})

export default router
