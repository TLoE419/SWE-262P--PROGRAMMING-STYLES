#!/usr/bin/env ruby
require 'set'

# Style #9: Continuation-Passing Style
# Each function takes an additional parameter (usually the last) which is another function
# That function parameter is applied at the end of the current function
# That function parameter is given as input what would be the output of the current function

# Read file and pass data to next function
def read_file(path_to_file, func)
  data = File.read(path_to_file)
  func.call(data, method(:normalize))
end

# Filter non-alphanumeric characters and pass to next function
def filter_chars(str_data, func)
  result = str_data.gsub(/[\W_]+/, ' ')
  func.call(result, method(:scan))
end

# Convert to lowercase and pass to next function
def normalize(str_data, func)
  func.call(str_data.downcase, method(:remove_stop_words))
end

# Split string into words and pass to next function
def scan(str_data, func)
  words = str_data.split
  func.call(words, method(:frequencies))
end

# Remove stop words and pass to next function
def remove_stop_words(word_list, func)
  stop_words_content = File.read('../stop_words.txt')
  stop_words = stop_words_content.split(',').to_set

  # Add single-letter words
  ('a'..'z').each { |c| stop_words.add(c) }

  filtered_words = word_list.select { |w| !stop_words.include?(w) && !w.empty? }
  func.call(filtered_words, method(:sort_func))
end

# Count word frequencies and pass to next function
def frequencies(word_list, func)
  wf = Hash.new(0)
  word_list.each { |w| wf[w] += 1 }
  func.call(wf, method(:print_text))
end

# Sort word frequencies and pass to next function
def sort_func(wf, func)
  sorted_wf = wf.sort_by { |k, v| -v }
  func.call(sorted_wf, method(:no_op))
end

# Print top 25 words and pass to next function
def print_text(word_freqs, func)
  limit = [25, word_freqs.size].min
  word_freqs[0...limit].each do |w, c|
    puts "#{w} - #{c}"
  end
  func.call
end

# No operation function
def no_op
  return
end

# Main function: kick off the continuation chain
if ARGV.length != 1
  puts "Usage: ruby eight.rb <text-file>"
  exit 1
end

read_file(ARGV[0], method(:filter_chars))
