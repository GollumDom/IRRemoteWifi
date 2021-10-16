import {Validator, Rule} from '../Validator';

export default function (
	{
		message
	}: {
		message?: string
	} = {}): Rule {
	return (v: any) => (v !== '' && v !== null && v !== false) || (Array.isArray(v) && v.length > 0) || message || 'Field required';
}
