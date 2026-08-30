A grep clone (WIP) to understand linux better

## Design:
    Line loader:
        - Given a file, loads the file into memory as 64kb chunks
        - As a particular pattern/line can be split across chunks, there is a BufReader
            interface that will dynamically manage the chunks as per need.
            Enforced property: if one chunk is discarded, then it wont be reloaded
        - Basically,
            Chunk -> BufReader -> Intermediate wrapper to ensure function reads only contiguous memory, and it cant see 
