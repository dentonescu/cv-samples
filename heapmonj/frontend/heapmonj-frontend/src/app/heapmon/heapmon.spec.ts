import { ComponentFixture, TestBed } from '@angular/core/testing';
import { provideHttpClientTesting } from '@angular/common/http/testing';
import { provideCharts, withDefaultRegisterables } from 'ng2-charts';

import { HeapmonComponent } from './heapmon.component';

describe('Heapmon', () => {
  let component: HeapmonComponent;
  let fixture: ComponentFixture<HeapmonComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [HeapmonComponent],
      providers: [
        provideHttpClientTesting(),
        provideCharts(withDefaultRegisterables())
      ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(HeapmonComponent);
    component = fixture.componentInstance;
    await fixture.whenStable();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
