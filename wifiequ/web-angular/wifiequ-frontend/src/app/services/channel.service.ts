import { Injectable } from "@angular/core";
import { Reading } from "../models/channels";

@Injectable({
    providedIn: 'root'
}) export class ChannelService {
    constructor() { }

    getDistinctChannelReadings(readings: Reading[]): Reading[] {
        const dict: Record<number, Reading> = {};
        for (const r of readings) {
            if (r.chan in dict) {
                dict[r.chan].dbm = (dict[r.chan].dbm + r.dbm) / 2.0;
            } else {
                dict[r.chan] = { ...r }; // don't store the original; it could be mutated elsewhere
            }
        }
        return Object.values(dict);
    }

    getSortedChannelReadings(readings: Reading[]): Reading[] {
        return readings.sort((a, b) => a.chan - b.chan);
    }
}