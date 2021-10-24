import Vue from 'vue';
import Vuetify from 'vuetify/lib/framework';
import fr from 'vuetify/src/locale/fr';
import { IconList } from './vicon';

Vue.use(Vuetify);

export default new Vuetify({
    icons: {
      values: {
        clear: IconList.mdiCloseCircle,
        radioOn: IconList.mdiRadioboxMarked,
        radioOff: IconList.mdiRadioboxBlank,
        expand: IconList.mdiChevronDown,
        file: IconList.mdiPaperclip,
      } 
    },
    lang: {
      locales: { fr },
      current: 'fr',
    },
});
