export interface Reading {
    chan: number;
    dbm: number;
}

export interface Channels {
    timestamp_ms: number;
    readings: Reading[];
}