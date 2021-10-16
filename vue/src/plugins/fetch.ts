import Vue from 'vue';
import { mdiWifi, mdiHome, mdiRemote } from '@mdi/js';

const constantMock = window.fetch;
window.fetch = function() {
	if (document.location.hostname === '127.0.0.1' || document.location.hostname === 'localhost') {
		arguments[0] = 'http://192.168.4.1' + arguments[0]
	}
	return (constantMock as any).apply(this, arguments)
}