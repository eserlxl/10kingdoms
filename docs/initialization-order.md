# Initialization Order

Generated: Sun Jan 18 10:05:32 +03 2026

## Initialization Sequence (from main())

  1. 	locale_res.init();
  1. 		config.init();
  1. 	config_adv.init();
  1. 	if( !cmd_line.init(argc, argv) )
  1. 			video.init();

