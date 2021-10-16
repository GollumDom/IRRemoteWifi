import required from './validation/required';
import ipv4 from './validation/ipv4';

export type Rule = (v: any) => boolean|string;

export class Validator {
	public readonly required = required;
	public readonly ipv4 = ipv4;
}
