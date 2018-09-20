# Questions

## What is pneumonoultramicroscopicsilicovolcanoconiosis?

A made up word of a nonexistent medical condition that is supposed to be the largest word in the English dictionary.

## According to its man page, what does `getrusage` do?

Returns a struct with statistics on resource usage.

## Per that same man page, how many members are in a variable of type `struct rusage`?

16

## Why do you think we pass `before` and `after` by reference (instead of by value) to `calculate`, even though we're not changing their contents?

To make the program quicker, otherwise the computer needs to make a local copy of thes variables for the function when the function is called.

## Explain as precisely as possible, in a paragraph or more, how `main` goes about reading words from a file. In other words, convince us that you indeed understand how that function's `for` loop works.

The loop first tests whether the character can be part of a word (is letter or non-initial apostrophed), it that is indeed the case, the character will be added to `word` and `index` will point to the first unused character in the string `word`. In the rare case that the word is longer than `LENGTH`, the word is dicarded by setting `index` to zero and advancing the file pointer to the next non-letter character.

If the current word is a word with a number, the word is ignored by setting `index` to zero and quickly advancing to the next non-alphanumeric character.

If the current character is not a letter or a number and the word has any size other than size (`index`) zero, it means we have found the first character after a word. `word` is terminated and `index` is made ready for a new word.

## Why do you think we used `fgetc` to read each word's characters one at a time rather than use `fscanf` with a format string like `"%s"` to read whole words at a time? Put another way, what problems might arise by relying on `fscanf` alone?

`fscanf` with `%s` will read strings, not words. They're not the same.

## Why do you think we declared the parameters for `check` and `load` as `const` (which means "constant")?

This is to prevent the function from changing the values of these variables in the scope of main. If we don't make these paramaters `const`, the only way to ensure that the function will not change these variables in the scope of main is to pass them as plain variables. As a side effect of that the function will make a local copy of these variables and the program will slow down.