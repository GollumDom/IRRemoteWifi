import {Validator, Rule} from '../Validator';

export default function (
	{
		message
	}: {
		message?: string
	} = {}): Rule {
	return (v: any) => {
		if (v == '') {
			return true
		}
		const split = v.split(/\./g);
		if (split.length === 4) {
			const n0 = parseInt(split[0]);
			const n1 = parseInt(split[1]);
			const n2 = parseInt(split[2]);
			const n3 = parseInt(split[3]);
			if (
				n0 == parseInt(split[0]) &&
				n1 == parseInt(split[1]) &&
				n2 == parseInt(split[2]) &&
				n3 == parseInt(split[3]) &&
				n0 >= 0 &&
				n1 >= 0 &&
				n2 >= 0 &&
				n3 >= 0 &&
				n0 <= 255 &&
				n1 <= 255 &&
				n2 <= 255 &&
				n3 <= 255
			) {
				return true;
			}
		}
		return 'IP address not valid';
	}
}
