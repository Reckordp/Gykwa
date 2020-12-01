ARG_ABSEN = %q[-v]
PRIOR = %w( ^/bin ^/usr/bin )

def system_diam(perintah)
	system(perintah, out: File::NULL, err: File::NULL)
end

def temukan_bin(nama_bin)
	kemajuan = `(whereis #{nama_bin})`.match(/#{nama_bin}\: ([\w\s\/\.]+)\n/)
	return if kemajuan.nil?
	kemajuan.captures.first.scan(/[\w\/\.]+/)
end

def ambil_bin(nama_bin)
	xct = temukan_bin(nama_bin).select { |x| system_diam("#{x} #{ARG_ABSEN}") rescue nil }
	return nil if xct.nil?
	prior = PRIOR.collect { |x| xct.select{ |i| i.match(/#{x}/) } }
	persetujuan = proc do |x, y, z|
		a = x.include?(y)
		z[y] += 1 if a
		a
	end

	xct.sort! do |a, b|
		c = [a, b]
		score = { a => 0, b => 0, langsung: false }
		prior.each do |x|
			if c.collect { |y| persetujuan.call(x, y, score) } .any?
				break score[:langsung] = true
			end
		end

		if score[:langsung]
			next 0 if score[a] == score[b]
			next -1
		end

		c.collect! { |y| y.scan(/\//).size }
		next c[0] <=> c[1]
	end
	xct.first
end

GCC = ambil_bin('gcc')
EXECUTABLE = "/tmp/%s"
GYKWA_SOURCE = EXECUTABLE % ['ReCk_Gykwa.o']
BGUI_SOURCE = EXECUTABLE % ['ReCk_BGUI.o']
PRLT_SOURCE = EXECUTABLE % ['ReCk_PRLT.o']

require 'benchmark'

task :default do
	bench = proc do |perintah|
		puts Benchmark.measure { sh(perintah) }
	end

	sh("%s -o %s -c Gykwa.c" % [GCC, GYKWA_SOURCE])
	sh("%s -o %s -c BangunGUI.c" % [GCC, BGUI_SOURCE])
	sh("%s -o %s -c Peralatan.c" % [GCC, PRLT_SOURCE])
	sh("%s -o %s %s -lX11 -lGL" % [GCC, EXECUTABLE % ['ReCk'], 
		[GYKWA_SOURCE, BGUI_SOURCE, PRLT_SOURCE].join(' ')])
	bench.call(EXECUTABLE % ['ReCk'])
end