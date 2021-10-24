<template>
	<span>{{ label }}</span>
</template>

<script lang="ts">
	import Vue from 'vue'

	const list = [
		'UNKNOWN',
		'PULSE_DISTANCE',
		'PULSE_WIDTH',
		'DENON',
		'DISH',
		'JVC',
		'LG',
		'LG2',
		'NEC',
		'PANASONIC',
		'KASEIKYO',
		'KASEIKYO_JVC',
		'KASEIKYO_DENON',
		'KASEIKYO_SHARP',
		'KASEIKYO_MITSUBISHI',
		'RC5',
		'RC6',
		'SAMSUNG',
		'SHARP',
		'SONY',
		'ONKYO',
		'APPLE',
		'BOSEWAVE',
		'LEGO_PF',
		'MAGIQUEST',
		'WHYNTER',
	];

	export default Vue.extend({
		name: 'Protocol',
		props: {
			protocol: { type: Number }
		},
		computed: {
			label(): string {
				return list[this.protocol] || 'UNKNOWN';
			}
		}
	})
</script>
