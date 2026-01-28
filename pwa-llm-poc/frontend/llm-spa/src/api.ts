const ENDPOINT_GENERATE = '/api/v1/generate';
const REQUEST_TIMEOUT_MS = 300000;

export type GenerateResponse = {
  output: string;
};

export type GenerateRequest = {
  prompt: string;
};

export async function generateText(
  prompt: string
): Promise<GenerateResponse> {
  const controller = new AbortController();
  const msTimeout = setTimeout(() => controller.abort(), REQUEST_TIMEOUT_MS);
  try {
    const response = await fetch(ENDPOINT_GENERATE, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ prompt } satisfies GenerateRequest),
      signal: controller.signal,
    });

    if (!response.ok) {
      const errorText = await response.text();
      throw new Error(errorText || `HTTP ${response.status}`);
    }

    return response.json();
  } catch (err: any) {
    throw new Error(`The backend appears to be unavailable. ${err.message}`);
  } finally {
    clearTimeout(msTimeout);
  }
}
