Dir.glob('../src/**/*').each{|filename|
    if filename.match('.cpp') then
        print filename.gsub('../', '').gsub('.cpp', '.o ')
    end
}