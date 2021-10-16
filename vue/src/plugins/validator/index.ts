import Vue from 'vue';
import { Validator } from './Validator';

declare module 'vue/types/vue' {
  interface Vue {
    $validator: Validator,
  }
}

declare module 'vue/types/options' {
	interface ComponentOptions<V extends Vue> {
		validator?: Validator;
	}
}

Object.defineProperty(Vue.prototype, '$validator', {
	get() {
		return this.$root.$options.validator;
	}
});

const validator = new Validator();
export default validator;

