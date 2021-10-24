<template>
	<v-simple-table v-if="signal">
		<tr><th class="text-left">Raw code        </th><td>{{ signal.raw_code.join(', ') }}</td></tr>
		<tr><th class="text-left">Protocol        </th><td><protocol :protocol="signal.received_ir_data.protocol"/></td></tr>
		<tr><th class="text-left">Number Of Bits  </th><td>{{ signal.received_ir_data.numberOfBits }}  </td></tr>
		<tr><th class="text-left">Protocol        </th><td>{{ signal.received_ir_data.protocol }}      </td></tr>
		<tr><th class="text-left">Decoded Raw Data</th><td>{{ signal.received_ir_data.decodedRawData }}</td></tr>
		<tr><th class="text-left">Command         </th><td>{{ signal.received_ir_data.command }}       </td></tr>
		<tr><th class="text-left">Address         </th><td>{{ signal.received_ir_data.address }}       </td></tr>
		<tr><th class="text-left">Extra           </th><td>{{ signal.received_ir_data.extra }}         </td></tr>
		<tr><th class="text-left">Flags           </th><td>{{ signal.received_ir_data.flags }}         </td></tr>
	</v-simple-table>
</template>

<script lang="ts">
	import Vue from 'vue'
	import Protocol from '../components/Protocol.vue'

	export default Vue.extend({
		name: 'Signal',
		components: {
			Protocol
		},
		props: {
			signal: {}
		},
	})
</script>
