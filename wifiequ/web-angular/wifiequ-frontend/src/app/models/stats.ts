export interface ChannelBin {
    chan: number;
    lower_freq_mhz: number;
    mid_freq_mhz: number;
    upper_freq_mhz: number;
}

export interface Stats {
    mode: 'mock' | 'live' | string; // any string, but mock and live are what I expect
    interface: string;
    refresh_ms: number;
    channels_defined: number;
    channel_bins: ChannelBin[];
}
