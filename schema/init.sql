CREATE TABLE IF NOT EXISTS news (
    id TEXT PRIMARY KEY,
    title TEXT NOT NULL,
    summary TEXT,
    body TEXT,
    url TEXT,
    category TEXT,
    people TEXT, -- Store as JSON string for simplicity
    date TEXT,
    lastmod TEXT
);

-- Optional initial data
