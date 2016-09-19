Dir.glob('src/**/*').each{|filename|
    if filename.match('.h') then
        base = File.basename(filename)
        `ln -snf ../#{filename} ./.includes/#{base}`
        puts filename
    end
}