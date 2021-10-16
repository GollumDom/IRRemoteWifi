import Vue from 'vue';
import App from './App.vue';
import router from './router';
import vuetify from './plugins/vuetify';
import validator from './plugins/validator';
import './plugins/fetch';
import './plugins/vicon';

Vue.config.productionTip = false

new Vue({
  router,
  vuetify,
  validator,
  render: h => h(App)
}).$mount('#app')
