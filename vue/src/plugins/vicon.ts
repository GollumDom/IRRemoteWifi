import Vue from 'vue';
import { 
	mdiWifi,
	mdiHome,
	mdiRemote,
	mdiWifiStrength1,
	mdiWifiStrength2,
	mdiWifiStrength3,
	mdiWifiStrength4,
	mdiLock,
	mdiCloseCircle,
	mdiEye,
	mdiEyeOff,
	mdiRadioboxBlank,
	mdiRadioboxMarked,
	mdiChevronDown,
	mdiContentSave,
	mdiDelete,
	mdiPaperclip,
	mdiPlus,
} from '@mdi/js';


export const IconList = {
	mdiWifi,
	mdiRemote,
	mdiHome,
	mdiWifiStrength1,
	mdiWifiStrength2,
	mdiWifiStrength3,
	mdiWifiStrength4,
	mdiLock,
	mdiCloseCircle,
	mdiEye,
	mdiEyeOff,
	mdiRadioboxBlank,
	mdiRadioboxMarked,
	mdiChevronDown,
	mdiContentSave,
	mdiDelete,
	mdiPaperclip,
	mdiPlus,
};

Vue.prototype.$icons = IconList;

declare module 'vue/types/vue' {
	interface Vue {
		$icon: keyof typeof IconList
	}
}
