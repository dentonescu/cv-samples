import { ComponentFixture, TestBed } from '@angular/core/testing';
import { EqualizerComponent } from './equalizer.component';

describe('Equalizer', () => {
    let component: EqualizerComponent;
    let fixture: ComponentFixture<EqualizerComponent>;

    beforeEach(async () => {
        await TestBed.configureTestingModule({
            imports: [EqualizerComponent]
        }).compileComponents();
        fixture = TestBed.createComponent(EqualizerComponent);
        component = fixture.componentInstance;
        await fixture.whenStable();
    });

    it('should instantiate', () => {
        expect(component).toBeTruthy();
    });
});